import matplotlib.pyplot as plt


def plot_epoch_mse(file_path="mse_data.txt", output_image_path="epoch_mse_plot.png"):
    epochs = []
    train_mse_values = []
    test_mse_values = []

    with open(file_path, "r") as f:
        # Skip lines until "Starting optimization using Gradient Descent..."
        for line in f:
            if "Starting optimization using Gradient Descent..." in line:
                break

        # Now read the epoch and MSE values
        for line in f:
            if "Optimization finished" in line:
                break
            try:
                parts = line.strip().split()
                if len(parts) == 3:
                    epoch = int(parts[0])
                    train_mse = float(parts[1])
                    test_mse = float(parts[2])
                    epochs.append(epoch)
                    train_mse_values.append(train_mse)
                    test_mse_values.append(test_mse)
            except ValueError:
                # Skip lines that don't conform to the expected "epoch train_mse test_mse" format
                continue

    if not epochs:
        print(f"No data found in {file_path} to plot.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(epochs, train_mse_values, marker="o", linestyle="-", label="Training MSE")
    plt.plot(epochs, test_mse_values, marker="x", linestyle="--", label="Test MSE")
    plt.title("Epoch vs. Mean Squared Error (MSE)")
    plt.xlabel("Epoch")
    plt.ylabel("MSE")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_image_path)
    print(f"Plot saved to {output_image_path}")


if __name__ == "__main__":
    plot_epoch_mse()
