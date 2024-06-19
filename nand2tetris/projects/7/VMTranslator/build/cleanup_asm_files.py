import os

# Paths
test_dir = 'test'
compare_dir = os.path.join(test_dir, 'compareFiles')

# Main cleanup function
def cleanup_asm_files():
    for root, dirs, files in os.walk(test_dir):
        # Skip the compareFiles directory
        if root.startswith(compare_dir):
            continue
        
        for file in files:
            if file.endswith('.asm'):
                asm_file = os.path.join(root, file)
                try:
                    os.remove(asm_file)
                    print(f"Deleted {asm_file}")
                except OSError as e:
                    print(f"Error deleting {asm_file}: {e}")

if __name__ == "__main__":
    cleanup_asm_files()

