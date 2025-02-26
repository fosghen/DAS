import socket
import numpy as np

def main():
    Ng, line_length, len_udp_pack, freq_send_data, pulse_width = 1.46, 60000, 20000, 1000, 100
    UDP_PORT = 8080
    ip_device = "192.168.1.10"
    socket_ = setup_socet(UDP_PORT)
    message = create_message(Ng, line_length, len_udp_pack, freq_send_data, pulse_width)
    send_message(socket_, message, ip_device, UDP_PORT)
    save_data(socket_, len_udp_pack)

def setup_socet(port: int) -> socket.socket:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("0.0.0.0", port))
    return sock
    
    
def create_message(Ng: float, line_length: int, len_udp_pack: int, freq_send_data: int, pulse_width: int) -> list[int]:
    message = []

    message.append(int(1000 * Ng) >> 8)
    message.append(int(1000 * Ng) & 0xff)

    message.append(line_length >> 8)
    message.append(line_length & 0xff)

    message.append(len_udp_pack >> 8)
    message.append(len_udp_pack & 0xff)

    message.append(freq_send_data >> 8)
    message.append(freq_send_data & 0xff)
    
    message.append(pulse_width >> 8)
    message.append(pulse_width & 0xff)

    return message

def send_message(sock: socket.socket, message: list[int], ip: str, port: int) -> None:
    sock.sendto(bytearray(message), (ip, port))
    
def save_data(sock: socket.socket, len_udp_pack: int) -> None:
    dt = np.dtype(np.int16)
    dt = dt.newbyteorder('>')
    array = np.zeros(100)
    index = 0
    prev_num_pack = 0
    while True:
        data = sock.recv(len_udp_pack + 5)
        num_pack = int(data[1] << 8 | data[2])
        if num_pack == 0:
            if index != 0:
                array.tofile(f"reflectogram_{index}.bin")
            prev_num_pack = num_pack
            array = np.frombuffer(data[5:], dtype=dt)
            index += 1
            
        elif (num_pack - prev_num_pack) == 1:
            prev_num_pack = num_pack
            array = np.hstack((array, np.frombuffer(data[5:], dtype=dt)))

if __name__ == "__main__":
    main()