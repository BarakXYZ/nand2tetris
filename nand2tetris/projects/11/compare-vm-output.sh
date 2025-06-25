#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "Comparing VM files..."

# Find all .vm files in the current directory structure
find . -name "*.vm" -not -path "./test-programs-vm-ref/*" | while read -r vm_file; do
    # Remove leading ./ from path
    relative_path=${vm_file#./}
    
    # Construct reference file path
    ref_file="test-programs-vm-ref/$relative_path"
    
    if [ ! -f "$ref_file" ]; then
        echo -e "${YELLOW}WARNING: Reference file not found: $ref_file${NC}"
        continue
    fi
    
    # Compare files
    if cmp -s "$vm_file" "$ref_file"; then
        echo -e "${GREEN}✓ MATCH: $relative_path${NC}"
    else
        echo -e "${RED}✗ MISMATCH: $relative_path${NC}"
        echo "  Generated: $vm_file"
        echo "  Reference: $ref_file"
        
        # Show first few lines of difference for debugging
        echo "  First difference:"
        diff -u "$ref_file" "$vm_file" | head -10 | sed 's/^/    /'
        echo
    fi
done

echo "Comparison complete."

