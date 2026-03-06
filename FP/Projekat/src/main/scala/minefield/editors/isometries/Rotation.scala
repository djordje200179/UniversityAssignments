package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield
import minefield.editors.Transforming.RotationDirection
import minefield.editors.Transforming.RotationDirection.*
import minefield.editors.{Isometry, Transforming}
import utils.{Dimensions, Location}

class Rotation(dimensions: Dimensions, direction: RotationDirection, anchor: Location) extends Isometry {
  val offset: Location = direction match {
    case Clockwise => Location(anchor.y - anchor.x, anchor.x + anchor.y - (dimensions.width - 1))
    case CounterClockwise => Location(anchor.y + anchor.x - (dimensions.height - 1), anchor.x - anchor.y)
  }

  def apply(minefield: Minefield): Minefield = {
    assert(dimensions == minefield.dimensions)
    Transforming.rotated(direction)(minefield)
  }

  def inverse: Isometry = Rotation(
    dimensions,
    direction match {
      case Clockwise => CounterClockwise
      case CounterClockwise => Clockwise
    },
    offset
  )
}
