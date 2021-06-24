import { Cord } from "./cord.js"
import Point from "./point.js"


class Interface {
    constructor(cvh) {
        this.cvh = cvh
        this.setEventListeners()
    }

    change_mode_event(value) {
        this.cvh.ctx.canvas.className = ""
        document.querySelectorAll("#panel_buttons button").forEach((el) => {
            el.classList.remove("active")
        })

        switch (value) {
            case this.cvh.modes.GRAB:
                document.getElementById("btn_move").classList.add("active")
                this.cvh.ctx.canvas.classList.add("grab")
                break
            case this.cvh.modes.ZOOM_IN:
                document.getElementById("btn_zoom_in").classList.add("active")
                this.cvh.ctx.canvas.classList.add("zoom_in")
                break
            case this.cvh.modes.ZOOM_OUT:
                document.getElementById("btn_zoom_out").classList.add("active")
                this.cvh.ctx.canvas.classList.add("zoom_out")
                break
            case this.cvh.modes.ROTATE:
                document.getElementById("btn_rotate").classList.add("active")
                this.cvh.ctx.canvas.classList.add("rotate")
                break
            case this.cvh.modes.ADD_POINT:
                document.getElementById("btn_add_point").classList.add("active")
                this.cvh.ctx.canvas.classList.add("add_point")
                break
        }
    }

    add_point_event(point) {
        let x = Math.round(point.cord.x * 1000) / 1000
        let y = Math.round(point.cord.y * 1000) / 1000

        let panel_object = $(`
            <div class="panel-object">
                <span class="panel-object_name">
                ${point.name}:
                </span>
                <div class="panel-object_point">
                <input type="tel" value="${x};${y}">
                </div>
                <div class="panel-object_state active" style='background:${point.color}'>
                <div class="menu">
                    <button class="btn delete"> Удалить </button>
                </div>
                </div>
            </div>   
        `)
        $(".panel_objects").append(panel_object).a


        panel_object.ready(() => {
            console.log(panel_object.find(".delete"))

            panel_object.find(".delete").click(() => {
                this.cvh.objects = this.cvh.objects.filter((a) => {
                    return a.name != point.name
                })

                panel_object.remove()
                this.cvh.changeObjectsEvent()
            })


            function fix_name() {
                let val = panel_object.find("input").val()

                let nums = val.split(";")
                if (nums.length != 2)
                    nums.length = 2

                nums.forEach((n, i, nums) => {
                    nums[i] = n.split('')
                        .filter(x => x >= '0' && x <= '9' || x == '-' || x == '.')
                        .join('')
                    if (parseFloat(nums[i]) == NaN)
                        nums[i] = parseFloat(nums[i]) || "0"


                })

                panel_object.find("input").val(nums.join(';'))

                point.cord.x = parseFloat(nums[0]) || 0

                point.cord.y = parseFloat(nums[1]) || 0

                if (nums.length != 2)
                    panel_object.find("input").addClass("error")
                else
                    panel_object.find("input").remove("error")
            }

            panel_object.find("input").keyup(() => {
                fix_name()
            })

            panel_object.find("input").change(() => {
                fix_name()
                panel_object.find("input").val(`${point.cord.x};${point.cord.y}`)
                this.cvh.changeObjectsEvent()

            })
        })

    }
    setEventListeners() {
        document.getElementById("btn_zoom_in").addEventListener("click", e => {
            this.cvh.mode = this.cvh.modes.ZOOM_IN
        })

        document.getElementById("btn_zoom_out").addEventListener("click", e => {
            this.cvh.mode = this.cvh.modes.ZOOM_OUT
        })

        document.getElementById("btn_move").addEventListener("click", e => {
            this.cvh.mode = this.cvh.modes.GRAB
        })
        document.getElementById("btn_rotate").addEventListener("click", e => {
            this.cvh.mode = this.cvh.modes.ROTATE
        })
        document.getElementById("btn_restore").addEventListener("click", e => {
            this.cvh.cs.restore()
        })
        document.getElementById("btn_add_point").addEventListener("click", e => {
            this.cvh.mode = this.cvh.modes.ADD_POINT
        })

        document.getElementById("btn_back").addEventListener("click", e => {
            this.cvh.back()
        })
        document.getElementById("btn_scale_solution").addEventListener("click", e => {
            this.cvh.auto_scale(true)
        })

        document.getElementById("btn_scale_all").addEventListener("click", e => {
            this.cvh.auto_scale(false)
        })
        
        


        document.getElementById("btn_add_point_to_list").addEventListener("click", e => {
            let point = new Point(new Cord(0, 0), this.cvh.cs)
            point.color = this.cvh.randomColor()
            point.name = "P" + this.cvh.getPointNum()
            this.cvh.add_point(point)
        })

        document.getElementById("btn_add_empty_list").addEventListener("click", e => {
            this.cvh.empty()
        })


        

    }


    render_result() {
        let res = "Нет решения";
        if (this.cvh.solution.solution) {
            let dots = this.cvh.solution.solution.map((p) =>
                "(" +
                Math.round(p.cord.x * 1000) / 1000
                + ";" +
                Math.round(p.cord.y * 1000) / 1000
                + ")").join('<br>')

            res = `Найден треугольник <br> ${dots}, <br> содержащий ${this.cvh.solution._solution_count} точек`

        }
        $(".panel .result").html(res)
    }

    clean_points()
    {
        $(".panel_objects").html('')
    }


}

export default Interface