from client import Client

def client_test():
    """
    >>> client = Client()
    >>> client.communicate("你好")
    '你也好'
    """

if __name__ == "__main__":
    import doctest
    doctest.testmod(verbose=True)
