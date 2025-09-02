import sys, socket 

host = "127.0.0.1"
port = 5000

with socket.create_connection((host, port)) as s:
    print(f"connected to {host}:{port}. Type lines, cntrl + c to quit")
    while True:
        try:
            line = input("> ")
        except (EOFError, KeyboardInterrupt):
            print("seeya")
            break
        if not line:
            continue
        tosend = line.encode() + b"\n"
        s.sendall(tosend)
        print(f"Sent: {line}")

        print(f"raw sent {tosend}")