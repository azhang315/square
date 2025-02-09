// Wait for the WASM module to initialize
Module.onRuntimeInitialized = function () {
    console.log("WASM Module Loaded!");

    const canvas = document.getElementById('canvas');
    // const ctx = canvas.getContext('2d'); // NOTE: you can NOT request two contexts back to back
    

    console.log("WASM Loaded");
    Module._test_server_log();

    const gl2 = canvas.getContext('webgl2');
    console.log("gl2 context: ")
    console.log(gl2); // should not be null


    // // Manually call main() from JS
    if (Module._main) {
    //     Module._main();
        console.log("main exists")
    }


};
