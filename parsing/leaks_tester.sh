!/bin/bash

# Function to check for leaks
check_leaks() {
    PID=$(pgrep minishell)  # Find the PID of minishell
    if [ -z "$PID" ]; then
        echo "Minishell is not running"
        exit 1
    fi

    while [ ! -z "$PID" ]; do
        echo "Checking leaks for minishell (PID: $PID)..."
        leaks $PID
        sleep 10  # Wait for 10 seconds before the next check
        PID=$(pgrep minishell)  # Recheck if minishell is still running
    done

    echo "Minishell process has ended."
}

# Run the leak checking function
check_leaks
