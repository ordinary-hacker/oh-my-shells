#!/bin/bash
# Package the default "shells" directory into multiple archive formats.

set -e

NAME="shells"
INDIR="$NAME"
OUTDIR="release"

# List of formats, human-readable name / command
declare -A FORMATS=(
    ["gzipped tarball (.tar.gz)"]="tar -czf {OUT} {IN}"
    ["zip archive (.zip)"]="zip -r {OUT} {IN}"
)

# Create the output directory
mkdir -p "$OUTDIR"

# Package for each archive format

echo "Packaging '$INDIR' into multiple formats..."
for FORMAT_NAME in "${!FORMATS[@]}"; do
    TEMPLATE="${FORMATS[$FORMAT_NAME]}"

    case "$FORMAT_NAME" in
        *tar.gz*) EXT=".tar.gz" ;;
        *.zip*)   EXT=".zip" ;;
        *)        EXT="" ;;   # fallback (shouldn't occur)
    esac

    OUTFILE="$OUTDIR/$NAME$EXT"

    echo "  -> $FORMAT_NAME ($OUTFILE)"

    CMD="${TEMPLATE//\{OUT\}/$OUTFILE}"
    CMD="${CMD//\{IN\}/$INDIR}"

    eval "$CMD"
done

echo "All done! Archives are in $OUTDIR."
