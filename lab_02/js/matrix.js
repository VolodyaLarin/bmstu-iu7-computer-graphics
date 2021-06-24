export default class Matrix {
    rows = 0
    cols = 0
    data = [[]]
    constructor(data) {
        if (!data) return
        this.data = data
        this.rows = data.length
        if (this.rows) this.cols = data[0].length
    }

    clear() {
        this.data = Array(this.rows)
        for (let i = 0; i < this.rows; i++) {
            this.data[i] = Array(this.cols)
            for (let j = 0; j < this.cols; j++)
                this.data[i][j] = 0
        }
    }

    static mul(m1, m2) {
        if (m1.cols != m2.rows) {
            throw "Incorrect sizes"
        }

        let res = new Matrix()

        res.rows = m1.rows
        res.cols = m2.cols
        res.clear()



        for (let row = 0; row < m1.rows; row++) {
            for (let k = 0; k < m1.cols; k++) {
                for (let col = 0; col < m2.cols; col++) {
                    res.data[row][col] += m1.data[row][k] * m2.data[k][col]
                }
            }
        }

        return res
    }

    static invert(matrix) {
        if (matrix.rows !== matrix.cols) {
            throw "matrix isn't square"
        }

        var i = 0, ii = 0, j = 0, dim = matrix.rows, active_el = 0, t = 0
        var inverted = [], copy = []
        for (i = 0; i < dim; i += 1) {
            inverted[inverted.length] = []
            copy[copy.length] = []
            for (j = 0; j < dim; j += 1) {
                if (i == j)
                    inverted[i][j] = 1
                else
                    inverted[i][j] = 0

                copy[i][j] = matrix.data[i][j]
            }
        }

        for (i = 0; i < dim; i += 1) {
            active_el = copy[i][i]

            if (active_el == 0) {
                for (ii = i + 1; ii < dim; ii += 1) {
                    if (copy[ii][i] != 0) {
                        for (j = 0; j < dim; j++) {
                            active_el = copy[i][j]
                            copy[i][j] = copy[ii][j]
                            copy[ii][j] = active_el
                            active_el = inverted[i][j]
                            inverted[i][j] = inverted[ii][j]
                            inverted[ii][j] = active_el
                        }
                        break
                    }
                }
                active_el = copy[i][i]
                if (active_el == 0) { return }
            }

            for (j = 0; j < dim; j++) {
                copy[i][j] = copy[i][j] / active_el
                inverted[i][j] = inverted[i][j] / active_el
            }

            for (ii = 0; ii < dim; ii++) {
                if (ii == i) { continue; }
                active_el = copy[ii][i]
                for (j = 0; j < dim; j++) {
                    copy[ii][j] -= active_el * copy[i][j]
                    inverted[ii][j] -= active_el * inverted[i][j]
                }
            }
        }

        return new Matrix(inverted)
    }
}



