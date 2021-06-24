import { Cord } from "./cord.js"

class Interface {

    cursor = new Cord(0, 0)
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
            this.cvh.cs_elephant.restore()
        })
        document.getElementById("btn_back").addEventListener("click", e => {
            this.cvh.back()
        })

        function fix_cord($input, format) {
            let val = $input.val()

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
            if (format)
                nums = nums.map((n) => String(parseFloat(n) || "0"))

            $input.val(nums.join(';'))

            if (nums.length != 2)
                $input.addClass("error")
            else
                $input.remove("error")
        }



        $(".cord-verify").change(function () {
            fix_cord($(this), true)
        })

        $(".cord-verify").keyup(function () {
            fix_cord($(this))
        })

        function fix_num($input, format) {
            let num = $input.val()

            num = num.split('')
                .filter(x => x >= '0' && x <= '9' || x == '-' || x == '.')
                .join('')


            if (parseFloat(num) == NaN)
                num = "0"

            if (format)
                num = String(parseFloat(num) || "0")


            $input.val(num)
        }

        $(".num-verify").change(function () {
            fix_num($(this), true)
        })

        $(".num-verify").keyup(function () {
            fix_num($(this))
        })

        $("#cursor_cord").change(() => {
            let nums = $("#cursor_cord").val().split(";").map((x) => parseFloat(x))

            this.cursor =
                new Cord(
                    nums[0],
                    nums[1]
                )

        })

        $("#rotate_btn").click(() => {
            let angle = - parseFloat($("#rotate_angle_input").val())
            
            this.cvh.save()
            this.cvh.cs_elephant.rotate(angle * Math.PI / 180,
                this.cvh.cs.getCanvasCord(this.cursor)
            )

        })

        $("#translate_btn").click(() => {
            let nums = $("#translate_cord_input").val().split(";").map((x) => parseFloat(x))

            let translate_cord =
                new Cord(
                    nums[0],
                    nums[1]
                )

            let cord_zero = this.cvh.cs.getCanvasCord(new Cord(0, 0))
            let cord_translate_cord = this.cvh.cs.getCanvasCord(translate_cord)


            let dx = cord_translate_cord.x - cord_zero.x
            let dy = cord_translate_cord.y - cord_zero.y

            this.cvh.save()
            this.cvh.cs_elephant.move(dx, dy)


        })

        $("#zoom_btn").click(() => {

            let mode = $("#zoom_form").serializeArray()[0].value


            let k = parseFloat($("#kof_input").val())

            let kx = k, ky = k
            if (mode == "x")
                ky = 1
            if (mode == "y")
                kx = 1
            
            this.cvh.save()
            this.cvh.cs_elephant.scaleXY(kx, ky, this.cvh.cs.getCanvasCord(this.cursor))
        })

        

    }




}

export default Interface