import matplotlib.pyplot as plt
import numpy as np
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

def plot_individual_accuracies(stats):
    runs = range(1, 21)  # 1 to 20
    labels = ['Info Gain\nBest Attr', 'Gini\nBest Attr', 
              'Info Gain\nRandom Top-3', 'Gini\nRandom Top-3']
    
    plt.figure(figsize=(15, 8))
    
    # Width of each bar and positions
    bar_width = 0.2
    r1 = np.arange(len(runs))
    r2 = [x + bar_width for x in r1]
    r3 = [x + bar_width for x in r2]
    r4 = [x + bar_width for x in r3]
    
    # Create bars
    plt.bar(r1, stats[:, 0], width=bar_width, label=labels[0], color='skyblue')
    plt.bar(r2, stats[:, 1], width=bar_width, label=labels[1], color='lightgreen')
    plt.bar(r3, stats[:, 2], width=bar_width, label=labels[2], color='salmon')
    plt.bar(r4, stats[:, 3], width=bar_width, label=labels[3], color='purple')
    
    plt.xlabel('Run Number')
    plt.ylabel('Accuracy (%)')
    plt.title('Accuracy by Run for Each Decision Tree Variant')
    plt.xticks([r + bar_width*1.5 for r in range(len(runs))], runs)
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    plt.tight_layout()
    plt.show()

def create_detailed_table(stats):
    # Create DataFrame with all runs
    runs = [f"Run {i+1}" for i in range(20)]
    df = pd.DataFrame(data=stats, 
                     columns=['Info Gain (Best)',
                             'Gini (Best)',
                             'Info Gain (Random)',
                             'Gini (Random)'],
                     index=runs)
    
    # Add mean and std at the bottom
    df.loc['Mean'] = df.mean()
    df.loc['Std Dev'] = df.std()
    
    # Format numbers to 2 decimal places
    return df.round(6)

# Read data from file
stats = read_accuracy_data('Graph/accuracy_results.txt')

# Plot chart
plot_individual_accuracies(stats)

# Create and display table
table = create_detailed_table(stats)
print("\nDetailed Accuracy Results Table:")
print(table)

# Save to CSV
table.to_csv('Graph/accuracy_results.csv')