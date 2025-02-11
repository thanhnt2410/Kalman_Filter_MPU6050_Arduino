import serial
import time
import csv

# Thiết lập cổng serial
SERIAL_PORT = "COM3"  # Thay bằng cổng serial của bạn
BAUD_RATE = 115200  # Tốc độ baud, cần khớp với thiết bị của bạn
TIMEOUT = 15  # Thời gian đọc dữ liệu (giây)
CSV_FILENAME = "serial_data.csv"  # Tên file CSV

def read_serial_data_and_save(port, baud_rate, timeout, filename):
    data_list = []  # Lưu dữ liệu đọc được
    start_time = time.time()  # Ghi lại thời điểm bắt đầu

    try:
        with serial.Serial(port, baud_rate, timeout=1) as ser:
            print(f"Đang đọc dữ liệu từ {port} trong {timeout} giây...")
            
            while (time.time() - start_time) < timeout:
                line = ser.readline().decode("utf-8", errors="ignore").strip()  # Đọc dữ liệu, bỏ lỗi
                if line:
                    print("Nhận được:", line)  # Debug: in dữ liệu thô nhận được

                    parts = line.split()  # Tách thành danh sách theo khoảng trắng
                    data_dict = {}

                    for part in parts:
                        if ":" in part:  # Chỉ xử lý dữ liệu có dạng key:value
                            try:
                                key, value = part.split(":")
                                data_dict[key] = float(value)  # Chuyển đổi sang số thực
                            except ValueError:
                                print(f"Bỏ qua dữ liệu lỗi: {part}")  # Debug lỗi

                    if data_dict:  # Nếu có dữ liệu hợp lệ, thêm vào danh sách
                        data_list.append(data_dict)

    except serial.SerialException as e:
        print(f"Lỗi cổng Serial: {e}")

    # Lưu vào file CSV nếu có dữ liệu hợp lệ
    if data_list:
        with open(filename, mode="w", newline="") as file:
            fieldnames = data_list[0].keys()  # Lấy tiêu đề cột từ dữ liệu đầu tiên
            writer = csv.DictWriter(file, fieldnames=fieldnames)
            writer.writeheader()  # Ghi tiêu đề cột
            writer.writerows(data_list)  # Ghi dữ liệu

        print(f"\nDữ liệu đã được lưu vào {filename}")

# Chạy chương trình
read_serial_data_and_save(SERIAL_PORT, BAUD_RATE, TIMEOUT, CSV_FILENAME)
