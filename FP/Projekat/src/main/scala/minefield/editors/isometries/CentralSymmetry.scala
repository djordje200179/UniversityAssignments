package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield
import minefield.editors.Transforming.RotationDirection.Clockwise
import minefield.editors.isometries.CentralSymmetry.Direction
import minefield.editors.{Isometry, Transforming}
import utils.{Dimensions, Location}

class CentralSymmetry(dimensions: Dimensions, direction: Direction) extends Isometry {
  override val offset: Location = direction match {
    case Direction.UpLeft => Location(-dimensions.height, -dimensions.width)
    case Direction.UpRight => Location(-dimensions.height, +dimensions.width)
    case Direction.DownLeft => Location(+dimensions.height, -dimensions.width)
    case Direction.DownRight => Location(+dimensions.height, +dimensions.width)
  }

  override def apply(minefield: Minefield): Minefield = {
    assert(minefield.dimensions == dimensions)

    (Transforming.rotated(Clockwise) andThen Transforming.rotated(Clockwise))(minefield)
  }

  override def inverse: Isometry = this
}

object CentralSymmetry {
  enum Direction {
    case UpLeft, UpRight, DownLeft, DownRight
  }
}
