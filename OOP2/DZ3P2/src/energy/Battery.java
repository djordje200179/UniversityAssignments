package energy;

import java.util.concurrent.atomic.AtomicInteger;

public class Battery {
	private final AtomicInteger currentEnergy;
	private final int capacity;

	public Battery(int capacity) {
		this.capacity = capacity;
		this.currentEnergy = new AtomicInteger(capacity);
	}

	public void addEnergy(int energy) {	currentEnergy.updateAndGet(old -> Math.min(old + energy, capacity)); }
	public void empty() { currentEnergy.set(0); }
	public boolean isFull() { return currentEnergy.get() == capacity; }
}
