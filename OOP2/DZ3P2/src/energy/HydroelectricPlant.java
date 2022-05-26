package energy;

import java.awt.*;

public class HydroelectricPlant extends Producer {
	private int surroundingWaterSurfaces = 0;

	public HydroelectricPlant(Battery battery) { super('H', Color.BLUE, 1500, battery); }

	@Override
	protected int producingEnergyUnits() { return surroundingWaterSurfaces; }
	@Override
	protected boolean isProductionSuccessful() { return surroundingWaterSurfaces > 0; }

	public void setSurroundingWaterSurfaces(int surroundingWaterSurfaces) { this.surroundingWaterSurfaces = surroundingWaterSurfaces; }
}
