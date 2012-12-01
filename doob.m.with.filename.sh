touch "$1"
cat "$1" | sh doob.m.sh | grep PSST: >> "$1"
