def load():
  print("Hello from python!")
  print(add_numbers(2, 4))
  say_hello("world")

def update(deltaTime):
  clear(0, (0, 0, 0, 255))
  draw_line(0, 10, 10, 100, 100, 2, (255, 255, 255, 255))
