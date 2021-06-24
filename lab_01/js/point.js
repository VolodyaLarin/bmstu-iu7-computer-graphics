import Drawer from "./drawer.js";

import { Cord } from "./cord.js";

class Point extends Drawer {
    cord = new Cord(0, 0)
    color = "#00F"
    width = 5

    constructor(cord, cs) {
        super()

        this.cord = cord
        this.cs = cs
    }

    render(ctx) {
        let cord = this.cs.getCanvasCord(this.cord);
        let width = this.width
        if (this.hover) {
            ctx.font = "14px sans-serif";
            let w = ctx.measureText(this.display_name).width
            ctx.fillStyle = "#000";


            ctx.fillRect(cord.x, cord.y - 5 , w + 15, 20)
            ctx.fillStyle = "#fff";
            ctx.fillText(this.display_name, cord.x + 10, cord.y + 10);
            width *= 1.5
        }
        ctx.fillStyle = this.color;
        ctx.beginPath()
        ctx.arc(cord.x, cord.y, width, 0, Math.PI * 2, true)
        ctx.fill()
        ctx.closePath()
        
    }

    get display_name(){
        return this.name + "(" + 
            Math.round(this.cord.x * 1000) / 1000 
            + ";" + 
            Math.round(this.cord.y * 1000) / 1000 
             + ")"
    }
}

export default Point