class Solution {
    solution = null
    _solution_count = 0
    cvh = null

    constructor(cvh) {
        this.cvh = cvh
    }

    point_in_treag(p0, p1, p2, p3) {
        let s1 = (p1.cord.x - p0.cord.x) * (p2.cord.y - p1.cord.y)
            - (p2.cord.x - p1.cord.x) * (p1.cord.y - p0.cord.y)
        let s2 = (p2.cord.x - p0.cord.x) * (p3.cord.y - p2.cord.y)
            - (p3.cord.x - p2.cord.x) * (p2.cord.y - p0.cord.y)
        let s3 = (p3.cord.x - p0.cord.x) * (p1.cord.y - p3.cord.y)
            - (p1.cord.x - p3.cord.x) * (p3.cord.y - p0.cord.y)

        return Math.sign(s1) == Math.sign(s2) && Math.sign(s2) == Math.sign(s3) && s1 != 0
    }
    count_points(p1, p2, p3) {
        let k = 0;
        this.cvh.objects.forEach(element => {
            if (this.point_in_treag(element, p1, p2, p3)) k++
        })
        return k
    }
    solute() {
        let n = this.cvh.objects.length

        this.solution = null
        this._solution_count = 0

        if (n < 3)
            return

        for (let i = 0; i < n - 2; i++) {
            for (let j = i + 1; j < n - 1; j++) {
                for (let k = j + 1; k < n; k++) {
                    let p1 = this.cvh.objects[i]
                    let p2 = this.cvh.objects[j]
                    let p3 = this.cvh.objects[k]

                    let nn = this.count_points(p1, p2, p3)

                    if (nn > this._solution_count) {
                        this._solution_count = nn
                        this.solution = [p1, p2, p3]
                    }
                }
            }
        }

        return this.solution
    }

    render(ctx) {
        if (!this.solution) {
            return
        }

        ([
            [this.solution[0], this.solution[1]],
            [this.solution[1], this.solution[2]],
            [this.solution[0], this.solution[2]]
        ].forEach((cords) => {
            
            let cord1 = this.cvh.cs.getCanvasCord(cords[0].cord)
            let cord2 = this.cvh.cs.getCanvasCord(cords[1].cord)
            ctx.lineWidth = 3;
            ctx.strokeStyle = "#f00"

            ctx.beginPath()
            ctx.moveTo(cord1.x, cord1.y)
            ctx.lineTo(cord2.x, cord2.y)
            ctx.stroke()
            ctx.closePath()
        }))


    }


}

export default Solution