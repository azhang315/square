// Wait for the WASM module to initialize
Module.onRuntimeInitialized = function () {
    console.log("WASM Module Loaded!");
    const canvas = document.getElementById('canvas');
    if (Module._main) {
        console.log("main exists")
    }
    else {
        console.log("main does not exist")
    }

    if (Module._test_server_log) {
        console.log("cross vm logging exposed")
        Module._test_server_log();
    }
    else {
        console.log("cross vm logging not exposed")
    }
    // Module._test_server_log();

    const gl = canvas.getContext('webgl2') || canvas.getContext('webgl');
    if (gl) {
        console.log("WebGL Version:", gl.getParameter(gl.VERSION));
    } else {
        console.log("Failed to initialize any WebGL context.");
    }
};
