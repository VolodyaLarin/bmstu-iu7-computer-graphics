import { Cord, CoordinateSystem } from "./cord.js"
import Grid from "./grid.js";
import Interface from "./interface.js";
import Point from "./point.js";
import Solution from "./solution.js";


class CanvasHelper {
    modes = {
        GRAB: "grab",
        ZOOM_IN: "zoom-in",
        ZOOM_OUT: "zoom-out",
        ROTATE: "rotate",
        ADD_POINT: "add_point",
    }


    history = null
    history_2 = null


    ctx = null
    canvas = null
    cs = null

    grid = null

    _mode = null
    _is_moving = false
    moving_first_point = new Cord(0, 0)
    moving_last_point = new Cord(0, 0)

    objects = []

    interface = null
    solution = null

    constructor(ctx, canvas) {
        this.interface = new Interface(this)
        this.solution = new Solution(this)
        

        this.ctx = ctx
        this.canvas = canvas
        this.cs = new CoordinateSystem(ctx)
        this.resize()

        this.grid = new Grid(this.cs)

        this.mode = this.modes.GRAB

        this.setListeners()
        this.cs.restore()
        this.changeObjectsEvent()


        this.tick()
    }
    tick() {
        this.render()

        window.requestAnimationFrame(this.tick.bind(this))
    }
    resize() {
        let dx = this.canvas.width - this.canvas.clientWidth
        let dy = this.canvas.height - this.canvas.clientHeight

        this.canvas.width = this.canvas.clientWidth
        this.canvas.height = this.canvas.clientHeight

        this.cs.move(- dx / 2, - dy / 2)
    }

    setListeners() {
        window.addEventListener("resize", () => {
            this.resize()
        })

        this.ctx.canvas.addEventListener('mousedown', e => {
            this.moving_first_point = new Cord(e.offsetX, e.offsetY)
            this.moving_last_point = new Cord(e.offsetX, e.offsetY)
            this.is_moving = true;
        });

        this.ctx.canvas.addEventListener('mousemove', e => {
            if (this.is_moving === true) {
                let dx = this.moving_last_point.x - e.offsetX
                let dy = this.moving_last_point.y - e.offsetY

                switch (this.mode) {
                    case this.modes.ROTATE:
                        let angle = Math.sign(dx) * Math.sqrt(dx * dx + dy * dy) / this.ctx.canvas.height * Math.PI * 2
                        console.log(angle)

                        this.cs.rotate(angle, this.moving_first_point);
                        break
                    case this.modes.GRAB:
                        this.cs.move(-dx, -dy);
                        break

                }
                this.moving_last_point = new Cord(e.offsetX, e.offsetY)
            }
            else {
                let borders = [
                    this.cs.getCordFromCanvas(new Cord(e.offsetX - 20, e.offsetY - 20)),
                    this.cs.getCordFromCanvas(new Cord(e.offsetX + 20, e.offsetY + 20))
                ]


                let min_x = Math.min(...borders.map(v => v.x))
                let max_x = Math.max(...borders.map(v => v.x))
                let min_y = Math.min(...borders.map(v => v.y))
                let max_y = Math.max(...borders.map(v => v.y))

                function in_border(cord, min_x, max_x, min_y, max_y) {
                    return (
                        cord.x >= min_x &&
                        cord.x <= max_x &&
                        cord.y >= min_y &&
                        cord.y <= max_y
                    )
                }

                function is_hover(obj) {
                    return obj instanceof Point
                        && in_border(obj.cord, min_x, max_x, min_y, max_y)
                }
                this.objects.forEach((obj) => obj.hover = is_hover(obj))
            }
        });

        window.addEventListener('mouseup', e => {
            if (this.is_moving === true) {

                let dx = this.moving_last_point.x - e.offsetX
                let dy = this.moving_last_point.y - e.offsetY

                switch (this.mode) {
                    case this.modes.ROTATE:
                        let angle = Math.sign(dx) * Math.sqrt(dx * dx + dy * dy) / this.ctx.canvas.height * Math.PI * 2
                        this.cs.rotate(angle, this.moving_first_point);
                        break
                    case this.modes.GRAB:
                        this.cs.move(-dx, -dy);
                        break
                    case this.modes.ZOOM_IN:
                        this.cs.scale(1.25, new Cord(e.offsetX, e.offsetY));
                        break
                    case this.modes.ZOOM_OUT:
                        this.cs.scale(0.8, new Cord(e.offsetX, e.offsetY));
                        break
                    case this.modes.ADD_POINT:
                        let point = new Point(
                            this.cs.getCordFromCanvas(
                                new Cord(
                                    e.offsetX,
                                    e.offsetY
                                )
                            ),
                            this.cs
                        );
                        point.color = this.randomColor()
                        point.name = "P" + this.getPointNum()
                        this.add_point(point)

                        break

                }
                this.is_moving = false;
            }
        });

    }
    render() {
        this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height)

        this.grid.render(this.ctx)

        this.solution.render(this.ctx)

        this.objects.forEach(obj => obj.render(this.ctx))



        if (this.is_moving)
            switch (this.mode) {
                case this.modes.ROTATE:
                    let point = new Point(
                        this.cs.getCordFromCanvas(this.moving_first_point),
                        this.cs
                    )
                    point.color = "#0f0"
                    point.width = 10
                    point.render(this.ctx)
            }


    }
    get is_moving() {
        return this._is_moving
    }

    set is_moving(value) {
        if (value)
            this.ctx.canvas.classList.add("active")
        else
            this.ctx.canvas.classList.remove("active")
        this._is_moving = !!value
    }

    get mode() {
        return this._mode
    }

    set mode(value) {
        this.interface.change_mode_event(value)
        this._mode = value
    }

    randomColor() {
        let colors = [
            "#d32f2f", "#C2185B", "#512DA8", "#3f51b5", "#2196f3", "#009688", "#4caf50", "#ffeb3b", "#ff5722", "#607d8b"
        ]

        return colors[Math.floor(Math.random() * colors.length)]
    }

    last_n = 0
    getPointNum() {
        this.last_n++;
        return this.last_n
    }

    add_point(point) {
        this.objects.push(point)
        this.interface.add_point_event(point)
        this.changeObjectsEvent()
    }

    changeObjectsEvent() {
        this.solution.solute()
        this.interface.render_result()

        let copy = []
        this.objects.forEach(
            (obj) => {
                let p = Object.create(
                    Object.getPrototypeOf(obj),
                    Object.getOwnPropertyDescriptors(obj)
                );
                p.cord = new Cord(obj.cord.x, obj.cord.y)
                copy.push(p)
            }
        )
        this.history = this.history_2
        this.history_2 = copy

    }

    auto_scale(only_res) {
        
        let objects = this.objects
        if (only_res)
            objects = this.solution.solution
        
        if (!objects.length) 
            return
        
        let min_x = Math.min(...objects.map((p) => p.cord.x))
        let max_x = Math.max(...objects.map((p) => p.cord.x))

        let min_y = Math.min(...objects.map((p) => p.cord.y))
        let max_y = Math.max(...objects.map((p) => p.cord.y))

        let x_c = (max_x + min_x) / 2
        let y_c = (max_y + min_y) / 2


        let scale = Math.min(
            this.ctx.canvas.width / (max_x - min_x),
            this.ctx.canvas.height / (max_y - min_y)
        ) * 0.95 

        if (scale == Infinity) scale = 50

        this.cs.setCenter(new Cord(x_c, y_c), scale)


    }

    back() {
        if (!this.history) {
            return
        }
        this.objects = this.history
        this.history = null
        this.changeObjectsEvent()

        this.solution.solute()

        this.render()

        this.interface.clean_points()
        this.objects.forEach((obj) => {
            this.interface.add_point_event(obj)
        })

        this.interface.render_result()

    }


    empty(){
        this.last_n = 0
        this.solution = new Solution(this)
        this.objects = []
        this.interface.clean_points()
        this.interface.render_result()

    }
}




export default CanvasHelper