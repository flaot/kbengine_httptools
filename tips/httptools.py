# -*- coding: utf-8 -*-

class HttpParserError(Exception):
    pass


class HttpParserCallbackError(HttpParserError):
    pass


class HttpParserInvalidStatusError(HttpParserError):
    pass


class HttpParserInvalidMethodError(HttpParserError):
    pass


class HttpParserInvalidURLError(HttpParserError):
    pass


class HttpParserUpgrade(Exception):
    pass


class HttpParser:

    def __init__(self, protocol: any, mode: int):
        """HttpParser

        protocol -- a Python object with the following methods
        (all optional):

          - on_message_begin()
          - on_header(name: bytes, value: bytes)
          - on_headers_complete()
          - on_body(body: bytes)
          - on_message_complete()
          - on_chunk_header()
          - on_chunk_complete()
          - on_status(status: bytes)
        """
        pass

    def on_header(self, header_name: bytes, header_value: bytes):
        print("on_header::header_name[%s] header_value[%s]" % (header_name, header_value))

    def on_headers_complete(self):
        print("on_headers_complete")

    def on_body(self, at: bytes):
        print("on_body:", at)

    def on_message_begin(self):
        print("on_message_begin")

    def on_message_complete(self):
        print("on_message_complete")

    def on_chunk_header(self):
        print("on_chunk_header")

    def on_chunk_complete(self):
        print("on_chunk_complete")

    def get_http_version(self) -> str:
        """Return an HTTP protocol version."""
        return "1.0"

    def should_keep_alive(self) -> bool:
        """Return ``True`` if keep-alive mode is preferred."""
        return False

    def should_upgrade(self) -> bool:
        """Return ``True`` if the parsed request is a valid Upgrade request.
           The method exposes a flag set just before on_headers_complete.
           Calling this method earlier will only yield `False`.
           """
        return False

    def feed_data(self, data: bytes):
        """Feed data to the parser.

        Will eventually trigger callbacks on the ``protocol``
        object.

        On HTTP upgrade, this method will raise an
        ``HttpParserUpgrade`` exception, with its sole argument
        set to the offset of the non-HTTP data in ``data``.
        """
        return None


class HttpRequestParser(HttpParser):

    def __init__(self, protocol: any):
        """HttpRequestParser

        protocol -- a Python object with the following methods
        (add optional):

          - on_url(url: bytes)
          - get_method()
        """
        HttpParser.__init__(self, protocol, 0)

    def on_url(self, at: bytes):
        print("on_url::", at)

    def get_method(self) -> str:
        """Return HTTP request method (GET, HEAD, etc)"""
        return ""


class HttpResponseParser(HttpParser):

    def __init__(self, protocol: any):
        """HttpResponseParser

        protocol -- a Python object with the following methods
        (add optional):

          - on_status(url: bytes)
          - get_status_code()
        """
        HttpParser.__init__(self, protocol, 1)

    def on_status(self, at: bytes):
        print("on_status", at)

    def get_status_code(self) -> int:
        """Return the status code of the HTTP response"""
        return 200


def parse_url(url: str) -> tuple:
    """Parse URL strings or bytes into a tuple.
    0-schema: str
    1-host: str
    2-port: int
    3-path: str
    4-query: str
    5-fragment: str
    6-userinfo: str
    """
    return "", "", 0, "", "", "", ""
