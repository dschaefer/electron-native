# Electron Native Project Example

This is an example of an Electron project that uses Native Node Addons written in C++.
It's a simple calculator that asynchronously adds two numbers and returns a promise to return the result.
In the native code, it uses the uv_work APIs to schedule a work item on the work queue and inject the results
into the main event loop to return it to the JavaScript world.

On the Electron side, this module is require'd by both the main process, that prints the results on the console,
and the renderer process which injects the results into the page.

We're using CMake here because it's just a better build system than node-gyp and we're planning on using
libraries that have CMake fragments.

Also, this is a bit of an introduction to C++11 for me. I'm using healthy doses of auto typing
and we send lambda's to the work queue. It makes the code much easier to read and understand.
