PUZZLE_PATH = "puzzle.txt"

f       = open(PUZZLE_PATH, 'r')
text    = ''.join(ch for ch in f.read() if not ch.isalnum())
symbols = sorted(set(text))

# Remove the extras:
symbols.remove('.')
symbols.remove('\n')

print(symbols)
f.close()
