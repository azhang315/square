from http.server import SimpleHTTPRequestHandler, HTTPServer

class CustomHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        super().end_headers()

PORT = 8000
server_address = ("", PORT)
httpd = HTTPServer(server_address, CustomHandler)

print(f"Serving on http://localhost:{PORT}")
httpd.serve_forever()

