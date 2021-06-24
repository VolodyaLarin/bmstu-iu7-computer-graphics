import { Cord } from "./cord.js";
import Drawer from "./drawer.js";

class Line {
    a = 0
    b = 0
    c = 0
    constructor(a, b, c) {
        this.a = a
        this.b = b
        this.c = c
    }
}

function get_line_k(point1, point2) {
    return new Line(
        point1.y - point2.y,
        point2.x - point1.x,
        point1.x * point2.y - point2.x * point1.y
    )
}

class Grid extends Drawer {
    color = "green"
    axias_color = "red"
    lineWidth = 0.5
    step = 1

    constructor(cs) {
        super()
        this.cs = cs
    }



    render(ctx) {

        let va = this.cs.visible_area

        let line_top = get_line_k(va[0], va[3])
        let line_bottom = get_line_k(va[1], va[2])
        let line_left = get_line_k(va[0], va[1])
        let line_right = get_line_k(va[3], va[2])

        let min_x = Math.min(...va.map(v => v.x))
        let max_x = Math.max(...va.map(v => v.x))

        let min_y = Math.min(...va.map(v => v.y))
        let max_y = Math.max(...va.map(v => v.y))


        min_x = Math.floor(min_x / this.step) * this.step
        min_y = Math.floor(min_y / this.step) * this.step

        max_x = Math.ceil(max_x / this.step) * this.step
        max_y = Math.ceil(max_y / this.step) * this.step

        ctx.lineWidth = this.lineWidth;


        for (let x = min_x; x < max_x; x += this.step) {
            if (x == 0) ctx.strokeStyle = this.axias_color
            else ctx.strokeStyle = this.color

            if (x % 10) ctx.lineWidth = this.lineWidth
            else ctx.lineWidth = this.lineWidth * 2

            let y1 = - (line_top.a * x + line_top.c) / line_top.b
            let y2 = - (line_bottom.a * x + line_bottom.c) / line_bottom.b

            let cord1 = this.cs.getCanvasCord(new Cord(x, y1))
            let cord2 = this.cs.getCanvasCord(new Cord(x, y2))

            ctx.beginPath()
            ctx.moveTo(cord1.x, cord1.y)
            ctx.lineTo(cord2.x, cord2.y)
            ctx.stroke()
            ctx.closePath()
        }

        for (let y = min_y; y < max_y; y += this.step) {
            if (y == 0) ctx.strokeStyle = this.axias_color
            else ctx.strokeStyle = this.color

            if (y % 10) ctx.lineWidth = this.lineWidth
            else ctx.lineWidth = this.lineWidth * 2

            let x1 = - (line_left.b * y + line_left.c) / line_left.a
            let x2 = - (line_right.b * y + line_right.c) / line_right.a

            let cord1 = this.cs.getCanvasCord(new Cord(x1, y))
            let cord2 = this.cs.getCanvasCord(new Cord(x2, y))

            ctx.beginPath()
            ctx.moveTo(cord1.x, cord1.y)
            ctx.lineTo(cord2.x, cord2.y)
            ctx.stroke()
            ctx.closePath()
        }

    }
}

export default Grid