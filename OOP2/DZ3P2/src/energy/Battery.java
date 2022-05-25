package energy;

public class Battery {
	private int currentEnergy;
	private final int capacity;

	public Battery(int capacity) {
		this.capacity = capacity;
		this.currentEnergy = capacity;
	}

	public void addEnergy(int energy) { currentEnergy = Math.max(currentEnergy + energy, capacity); }
	public void empty() { currentEnergy = 0; }
	public boolean isFull() { return currentEnergy == capacity; }
}
