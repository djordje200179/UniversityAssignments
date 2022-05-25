package energy;

import java.awt.*;

public class Hydroplant extends Producer {
	private int surroundingWaterSurfaces = 0;

	public Hydroplant(Battery battery) { super('H', Color.BLUE, 1500, battery); }

	protected int producingEnergyUnits() { return surroundingWaterSurfaces; }
	protected boolean isProductionSuccessful() { return surroundingWaterSurfaces > 0; }

	public void setSurroundingWaterSurfaces(int surroundingWaterSurfaces) { this.surroundingWaterSurfaces = surroundingWaterSurfaces; }
}
