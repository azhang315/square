if (!navigator.gpu) {
    alert("WebGPU is not supported in this browser.");
}

let moduleInstance;
let canvas = document.getElementById("canvas");
let ctx = canvas.getContext("webgpu");

// Setup WebGPU
async function setupWebGPU() {
    const adapter = await navigator.gpu.requestAdapter();
    const device = await adapter.requestDevice();
    const context = canvas.getContext("webgpu");
    context.configure({
        device: device,
        format: "bgra8unorm"
    });

    return { device, context };
}

// Renders WASM pixel buffer onto canvas
async function renderCanvas() {
    const { device, context } = await setupWebGPU();

    async function drawCanvas() {
        let pixelBuffer = new Uint32Array(moduleInstance._getPixelBuffer());
        let imageData = new ImageData(new Uint8ClampedArray(pixelBuffer.buffer), 500, 500);

        let bitmap = await createImageBitmap(imageData);
        let ctx2D = canvas.getContext("2d");
        ctx2D.drawImage(bitmap, 0, 0);

        requestAnimationFrame(drawCanvas);
    }

    requestAnimationFrame(drawCanvas);
}

// Load WASM Module
async function loadWASM() {
    Module = {
        onRuntimeInitialized: function () {
            moduleInstance = Module;
            moduleInstance._startMainLoop();  // Start Emscripten's main loop
            renderCanvas();
        }
    };
}

loadWASM();
