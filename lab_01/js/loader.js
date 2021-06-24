import CanvasHelper from "./canvas_helper.js"



window.addEventListener("load", () => {
    let canvas = document.getElementById('canvas')

    let ctx = canvas.getContext("2d")
    let cvh = new CanvasHelper(ctx, canvas)
    console.log(cvh)
})

