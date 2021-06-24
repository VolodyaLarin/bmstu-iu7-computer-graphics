import Matrix from "./matrix.js"

class Cord {
    x = 0
    y = 0
    constructor(x, y) {
        this.x = x
        this.y = y
    }
}
class CoordinateSystem {
    ctx = null
    matrix = new Matrix([
        [1, 0, 0],
        [0, 1, 0],
        [0, 0, 1]
    ])

    constructor(ctx) {
        this.ctx = ctx
        this.restore()
    }

    restore() {
        let dx = this.ctx.canvas.width / 2
        let dy = this.ctx.canvas.height / 2
        this.matrix =
            new Matrix([
                [15, 0, 0],
                [0, -15, 0],
                [dx, dy, 1]
            ])
    }

    setCenter(cord, scale) {
        this.matrix =
            new Matrix([
                [scale, 0, 0],
                [0, -scale, 0],
                [-cord.x * scale, cord.y * scale, 1]
            ])

        let shift = new Cord(
            this.ctx.canvas.width / 2,
            this.ctx.canvas.height / 2
        )

        this.move(shift.x, shift.y)


    }
    move(dx, dy) {
       
        this.matrix = Matrix.mul(
            this.matrix,
            new Matrix([
                [1, 0, 0],
                [0, 1, 0],
                [dx, dy, 1]
            ]))
    }
    rotate(angle, cord) {
        if (!cord)
            cord = new Cord(
                this.ctx.canvas.width / 2,
                this.ctx.canvas.height / 2
            )


        this.move(-cord.x, -cord.y)

        this.matrix = Matrix.mul(
            this.matrix,
            new Matrix([
                [Math.cos(angle), Math.sin(angle), 0],
                [-Math.sin(angle), Math.cos(angle), 0],
                [0, 0, 1]]
            ))

        this.move(cord.x, cord.y)
    }

    scale(k, cord) {
        if (!cord)
            cord = new Cord(
                this.ctx.canvas.width / 2,
                this.ctx.canvas.height / 2
            )

        let i = 0
        let q = Math.pow(k, 0.1)
        
        // if (k < 1 && this.matrix.data[0][0] < 5)
        //     return

        let interv = setInterval(() => {
            this.move(-cord.x, -cord.y)
            this.matrix = Matrix.mul(
                this.matrix,
                new Matrix([
                    [q, 0, 0],
                    [0, q, 0],
                    [0, 0, 1]]
                ))

            this.move(cord.x, cord.y)

            i++
            if (i >= 10)
                clearInterval(interv)

        }, 10)

    }


    scaleXY(x, y, cord) {
            this.move(-cord.x, -cord.y)
            this.matrix = Matrix.mul(
                this.matrix,
                new Matrix([
                    [x, 0, 0],
                    [0, y, 0],
                    [0, 0, 1]]
                ))

            this.move(cord.x, cord.y) 
    }

    getCanvasCord(cord) {
        let m = new Matrix([[cord.x, cord.y, 1]])
        let res = Matrix.mul(m, this.matrix)
        let x = res.data[0][0] / res.data[0][2]
        let y = res.data[0][1] / res.data[0][2]
        return new Cord(x, y)
    }

    get invert_matrix() {
        return Matrix.invert(this.matrix)
    }

    getCordFromCanvas(cord) {
        let m = new Matrix([[cord.x, cord.y, 1]])
        let res = Matrix.mul(m, this.invert_matrix)
        let x = res.data[0][0] / res.data[0][2]
        let y = res.data[0][1] / res.data[0][2]
        return new Cord(x, y)
    }

    get visible_area() {
        return [
            this.getCordFromCanvas(new Cord(0, 0)),
            this.getCordFromCanvas(new Cord(0, this.ctx.canvas.height)),
            this.getCordFromCanvas(new Cord(this.ctx.canvas.width, this.ctx.canvas.height)),
            this.getCordFromCanvas(new Cord(this.ctx.canvas.width, 0))
        ]
    }

}


export { Cord, CoordinateSystem }