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
        vm_files = [file for file in files if file.endswith('.vm')]

        if vm_files:
            if len(vm_files) == 1:
                vm_file = os.path.join(root, vm_files[0])
                asm_file = vm_file.replace('.vm', '.asm')

                # Run VMTranslator on the single .vm file
                print(f"Running VMTranslator on {vm_file}")
                if run_vm_translator(vm_file) != 0:
                    all_tests_passed = False
                    continue

                # Compare generated .asm file with reference .asm file
                reference_asm_file = os.path.join(compare_dir, os.path.basename(asm_file))
                if compare_files(asm_file, reference_asm_file):
                    print(f"Test passed for {vm_file}")
                else:
                    print(f"Test failed for {vm_file}: {asm_file} != {reference_asm_file}")
                    all_tests_passed = False

            else:
                # Run VMTranslator on the directory containing multiple .vm files
                print(f"Running VMTranslator on directory {root}")
                if run_vm_translator(root) != 0:
                    all_tests_passed = False
                    continue

                for vm_file in vm_files:
                    asm_file = os.path.join(root, vm_file.replace('.vm', '.asm'))

                    # Compare generated .asm file with reference .asm file
                    reference_asm_file = os.path.join(compare_dir, os.path.basename(asm_file))
                    if compare_files(asm_file, reference_asm_file):
                        print(f"Test passed for {os.path.join(root, vm_file)}")
                    else:
                        print(f"Test failed for {os.path.join(root, vm_file)}: {asm_file} != {reference_asm_file}")
                        all_tests_passed = False

    if all_tests_passed:
        print("All tests passed!")
    else:
        print("Some tests failed.")

if __name__ == "__main__":
    main()

