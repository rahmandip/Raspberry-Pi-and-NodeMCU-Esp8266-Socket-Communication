import socket

nodemcu_ip = '192.168.x.x'  # Replace with your NodeMCU's IP address
nodemcu_port = 80
buffer_size = 1024

def send_request(command):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((nodemcu_ip, nodemcu_port))
        client_socket.sendall((command + "\r\n").encode())
        response = client_socket.recv(buffer_size)
        return response.decode()

while True:
    cmd = input("Enter command (TEMP/HUM): ").upper()
    if cmd in ['TEMP', 'HUM']:
        response = send_request(cmd)
        print("Response from NodeMCU:", response)
    else:
        print("Invalid command, please enter TEMP or HUM.")
