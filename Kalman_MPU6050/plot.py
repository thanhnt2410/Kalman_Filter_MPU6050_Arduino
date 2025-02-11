import pandas as pd
import matplotlib.pyplot as plt

# Đọc dữ liệu từ file CSV
CSV_FILENAME = "serial_data.csv"  # Tên file CSV

# Đọc dữ liệu vào DataFrame
df = pd.read_csv(CSV_FILENAME)

# Kiểm tra dữ liệu
print("Dữ liệu từ CSV:")
print(df.head())

# Vẽ biểu đồ
plt.figure(figsize=(10, 5))
plt.plot(df.index, df["PitchY"], label="PitchY", marker="o", linestyle="-")
plt.plot(df.index, df["RollX"], label="RollX", marker="s", linestyle="--")

# Thiết lập tiêu đề và nhãn
plt.title("Biểu đồ PitchY và RollX từ dữ liệu Serial")
plt.xlabel("Số lần đọc")
plt.ylabel("Giá trị")
plt.legend()
plt.grid()

# Hiển thị biểu đồ
plt.show()
