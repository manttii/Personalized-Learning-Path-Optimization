#!/usr/bin/env python3
"""
Personalized Learning Path Optimization - Web Visualizer Server
Graphic Era (Deemed to be University) - 3rd Sem PBL
"""

import http.server
import socketserver
import webbrowser
import os
import sys

PORT = 8080
DIRECTORY = os.path.dirname(os.path.abspath(__file__))

class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

def run():
    os.chdir(DIRECTORY)
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        url = f"http://localhost:{PORT}/index.html"
        print("=" * 70)
        print("  Personalized Learning Path Optimization - Visualizer Server")
        print(f"  Server listening on: {url}")
        print("  Press Ctrl+C to stop the server.")
        print("=" * 70)
        try:
            webbrowser.open(url)
        except Exception:
            pass
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer shutting down.")

if __name__ == "__main__":
    run()
