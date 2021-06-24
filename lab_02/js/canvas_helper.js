import { Cord, CoordinateSystem } from "./cord.js"
import Grid from "./grid.js";
import Interface from "./interface.js";
import Line from "./line.js";
import Point from "./point.js";


class CanvasHelper {
    modes = {
        GRAB: "grab",
        ZOOM_IN: "zoom-in",
        ZOOM_OUT: "zoom-out",
        ROTATE: "rotate",
    }


    ctx = null
    canvas = null
    cs = null
    cs_elephant = null

    grid = null

    _mode = null
    _is_moving = false
    moving_first_point = new Cord(0, 0)
    moving_last_point = new Cord(0, 0)

    objects = []

    interface = null

    history = []

    back() {
        if (!this.history.length)
            return

        let back_matrixes = this.history.pop()

        this.cs.matrix = back_matrixes[0]
        this.cs_elephant.matrix = back_matrixes[1]
    }

    save() {
        this.history.push([
            this.cs.matrix,
            this.cs_elephant.matrix
        ])
    }

    constructor(ctx, canvas) {
        this.interface = new Interface(this)

        fetch("./picture.json").then((data) => {
            let fix_point_x = (x) => x / 20 - 18
            let fix_point_y = (y) => - y / 20 + 14
            data.json().then((json) => {
                this.objects = [...json.map(
                    l =>
                        l.length == 1 ?
                            ((p) => {
                                let point = new Point(
                                    new Cord(
                                        fix_point_x(p[0]),
                                        fix_point_y(p[1]
                                        )
                                    ),
                                    this.cs_elephant
                                );
                                point.width = 7
                                point.color = "#000"
                                return point
                            })(l[0]) :
                            new Line(
                                ...l.map(
                                    k =>
                                        new Cord(fix_point_x(k[0]), fix_point_y(k[1]))
                                ),
                                this.cs_elephant
                            )
                )]
            })
        })

        this.ctx = ctx
        this.canvas = canvas
        this.cs = new CoordinateSystem(ctx)
        this.cs_elephant = new CoordinateSystem(ctx)
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
        this.cs_elephant.move(- dx / 2, - dy / 2)
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
                        let angle = Math.sign(dx) * Math.sign(this.moving_first_point.y - e.offsetY) * Math.sqrt(dx * dx + dy * dy) / this.ctx.canvas.height * Math.PI * 2
                        console.log(angle)

                        this.cs.rotate(angle, this.moving_first_point);
                        this.cs_elephant.rotate(angle, this.moving_first_point);
                        break
                    case this.modes.GRAB:
                        this.cs.move(-dx, -dy);
                        this.cs_elephant.move(-dx, -dy);
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
                        this.cs_elephant.rotate(angle, this.moving_first_point);
                        break
                    case this.modes.GRAB:
                        this.cs.move(-dx, -dy);
                        this.cs_elephant.move(-dx, -dy);
                        break
                    case this.modes.ZOOM_IN:
                        this.cs.scale(1.25, new Cord(e.offsetX, e.offsetY));
                        this.cs_elephant.scale(1.25, new Cord(e.offsetX, e.offsetY));
                        break
                    case this.modes.ZOOM_OUT:
                        this.cs.scale(0.8, new Cord(e.offsetX, e.offsetY));
                        this.cs_elephant.scale(0.8, new Cord(e.offsetX, e.offsetY));

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

    changeObjectsEvent() { }

}




export default CanvasHelper