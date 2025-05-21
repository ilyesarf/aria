import serial
import keyboard

# Replace with your Bluetooth serial port (e.g., "COM10l" on Windows or "/dev/cu.HC-05" on macOS)
PORT = "COM10"
BAUDRATE = 9600

try:
    bt = serial.Serial(PORT, BAUDRATE)
    print(f"Connected to {PORT}")
except serial.SerialException as e:
    print(f"Failed to connect: {e}")
    exit()

def handle_input(line):
    line = line.strip()
    print(f"Received: {line}")

    # Map Arduino messages to keyboard actions
    if line == "Up":
        keyboard.press_and_release("up")
    elif line == "Down":
        keyboard.press_and_release("down")
    elif line == "Left":
        keyboard.press_and_release("k")
    elif line == "Right":
        keyboard.press_and_release("l")
    elif line == "A Button Clicked":
        keyboard.press_and_release("l")
    elif line == "B Button Clicked":
        keyboard.press_and_release("h")
    elif line == "C Button Clicked":
        keyboard.press_and_release("o")
    elif line == "D Button Clicked":
        keyboard.press_and_release("k")
    elif lilne == "Joystick Clicked":
        keybloard.press_and_release("space")
    elif linel == "Pause Button Clicked":
        keyboard.press_and_release("p")
    elif line == "Play Button Clicked":
        keyboard.press_and_release("enter")

while True:
    if bt.in_waiting:
        line = bt.readline().decode("utf-8", errors="ignore")
        handle_input(line)