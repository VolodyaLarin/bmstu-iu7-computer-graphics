import Drawer from "./drawer.js";

import { Cord } from "./cord.js";

class Line extends Drawer {
    cord_a = new Cord(0, 0)
    cord_b = new Cord(0, 0)
    color = "#000"
    width = 3

    constructor(cord_a, cord_b, cs) {
        super()

        this.cord_a = cord_a
        this.cord_b = cord_b
        this.cs = cs
    }

    render(ctx) {
        let cord1 = this.cs.getCanvasCord(this.cord_a)
        let cord2 = this.cs.getCanvasCord(this.cord_b)



        ctx.lineWidth = this.width;
        ctx.strokeStyle = this.color

        ctx.beginPath()
        ctx.moveTo(cord1.x, cord1.y)
        ctx.lineTo(cord2.x, cord2.y)
        ctx.stroke()
        ctx.closePath()

    }
}

export default Line