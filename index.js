import Module from './app.js';

async function startApp() {
    const app = await Module();  // Emscripten module initialization
    const game = new app.Application();

    // Run the app
    game.run();

    // Connect JS events to WASM
    document.getElementById('canvas').addEventListener('mousedown', (e) => {
        game.onMouseDown(e.clientX, e.clientY);
    });
}

startApp();
