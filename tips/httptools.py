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

    def __init__(self, obj: any, mode: int):
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

    def get_http_version(self):
        return 1.0

    def should_keep_alive(self):
        return False

    def should_upgrade(self):
        return False

    def feed_data(self, data: bytes):
        return None


class HttpRequestParser(HttpParser):

    def __init__(self, obj: any):
        HttpParser.__init__(self, obj, 0)

    def on_url(self, at: bytes):
        print("on_url::", at)

    def get_method(self):
        return ""


class HttpResponseParser(HttpParser):

    def __init__(self, obj: any):
        HttpParser.__init__(self, obj, 1)

    def on_status(self, at: bytes):
        print("on_status", at)

    def get_status_code(self) -> bool:
        return False


def parse_url(url: str) -> tuple:
    """
    解析http地址信息
    0-schema: str
    1-host: str
    2-port: int
    3-path: str
    4-query: str
    5-fragment: str
    6-userinfo: str

    :rtype: tuple
    :param url: 地址栏输入的地址
    """
    return "", "", 0, "", "", "", ""
