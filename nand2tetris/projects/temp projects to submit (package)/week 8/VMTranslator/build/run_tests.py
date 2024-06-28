import os
import subprocess
import filecmp

# Paths
vm_translator_path = './VMTranslator'
test_dir = 'test'
compare_dir = os.path.join(test_dir, 'compareFiles')

# Helper function to run the VMTranslator
def run_vm_translator(target):
    command = [vm_translator_path, target]
    print(f"Running command: {' '.join(command)}")  # Debug: Print the command being run
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running VMTranslator on {target}: {result.stderr}")
    return result.returncode

# Helper function to compare files
def compare_files(generated_file, reference_file):
    return filecmp.cmp(generated_file, reference_file, shallow=False)

# Main test runner
def main():
    all_tests_passed = True

    # Iterate through test folders
    for root, dirs, files in os.walk(test_dir):
        if root == compare_dir:
            continue  # Skip the compareFiles directory

        vm_files = [file for file in files if file.endswith('.vm')]
        contains_sys_vm = any(file == 'Sys.vm' for file in vm_files)

        if vm_files:
            folder_name = os.path.basename(root)
            if len(vm_files) == 1 and not contains_sys_vm:
                vm_file = os.path.join(root, vm_files[0])
                asm_file = os.path.join(root, vm_files[0].replace('.vm', '.asm'))

                # Run VMTranslator on the single .vm file
                print(f"Running VMTranslator on {vm_file}")
                if run_vm_translator(vm_file) != 0:
                    all_tests_passed = False
                    continue

            else:
                # Run VMTranslator on the directory containing multiple .vm files or Sys.vm
                asm_file = os.path.join(root, folder_name + '.asm')
                print(f"Running VMTranslator on directory {root}")
                if run_vm_translator(root) != 0:
                    all_tests_passed = False
                    continue

            # Debug: Check if the expected .asm file exists before comparing
            if not os.path.exists(asm_file):
                print(f"Expected .asm file does not exist: {asm_file}")
                all_tests_passed = False
                continue

            # Compare generated .asm file with reference .asm file
            reference_asm_file = os.path.join(compare_dir, os.path.basename(asm_file))
            if compare_files(asm_file, reference_asm_file):
                print(f"Test passed for {root}")
            else:
                print(f"Test failed for {root}: {asm_file} != {reference_asm_file}")
                all_tests_passed = False

    if all_tests_passed:
        print("All tests passed!")
    else:
        print("Some tests failed.")

if __name__ == "__main__":
    main()

