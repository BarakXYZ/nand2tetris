processed_dirs=""
for dir in */; do
    if [ -d "$dir" ] && [ "$(basename "$dir")" != "JackAnalyzer" ] && [ "$(basename "$dir")" != "test-programs-vm-ref" ]; then
        ./JackAnalyzer/build/JackAnalyzer "$dir"
        processed_dirs="$processed_dirs $(basename "$dir")"
    fi
done
echo "Processed folders:$processed_dirs"
