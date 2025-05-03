import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def read_accuracy_data(filename):
    # Read the data from file
    stats = []
    with open(filename, 'r') as file:
        for line in file:
            # Convert each line to array of 4 float numbers
            values = [float(x) for x in line.strip().split()]
            stats.append(values)
    return np.array(stats)
data=read_accuracy_data('Graph/accuracy_results.txt')
# Calculate means and standard deviations
means = np.mean(data, axis=0)
stds = np.std(data, axis=0)

# Create column chart
plt.figure(figsize=(10, 6))
x = np.arange(4)
methods = ['Info Gain\nBest Attr', 'Gini\nBest Attr', 'Info Gain\nRandom Top-3', 'Gini\nRandom Top-3']

plt.bar(x, means, yerr=stds, capsize=5)
plt.ylabel('Accuracy (%)')
plt.title('Average Accuracy by Method over 20 Runs')
plt.xticks(x, methods)
plt.grid(True, axis='y', linestyle='--')

# Add value labels on top of bars
for i, v in enumerate(means):
    plt.text(i, v + stds[i] + 0.5, f'{v:.4f}%', ha='center')

plt.tight_layout()
plt.show()

# Create table
df = pd.DataFrame({
    'Method': ['Best Attribute', 'Random Top-3'],
    'Information Gain': [means[0], means[2]],
    'Gini Impurity': [means[1], means[3]]
})

# Format accuracy values
df['Information Gain'] = df['Information Gain'].map('{:.6f}%'.format)
df['Gini Impurity'] = df['Gini Impurity'].map('{:.6f}%'.format)

print("\nAccuracy Results Table:")
print("=" * 60)
print(df.to_string(index=False))
print("=" * 60)
