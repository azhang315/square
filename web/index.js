// Wait for the WASM module to initialize
Module.onRuntimeInitialized = function () {
    console.log("WASM Module Loaded!");

    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');

    console.log("WASM Loaded");
    Module._test_server_log();


    // // Manually call main() from JS
    if (Module._main) {
    //     Module._main();
        console.log("main exists")
    }

    // // Test log to verify server-side logging
    Module._test_server_log();
    Module._test_server_log();
    Module._test_server_log();
};
