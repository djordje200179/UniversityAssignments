package com.djordjemilanovic.minesweeper
package minefield

import minefield.Minefield.Cell
import minefield.Minefield.Cell.*
import utils.{Dimensions, Location, Sector}

import java.lang.Object
import scala.util.Try

case class Minefield(mines: Vector[Vector[Cell]]) {
  val dimensions = Dimensions(
    height = mines.length,
    width = if mines.nonEmpty then mines(0).length else 0
  )
  val minesCount: Int = mines.flatten.count(Mine.equals)

  val difficulty: Difficulty = Difficulty.values.find { difficulty =>
    dimensions.area <= difficulty.maxArea && minesCount <= difficulty.maxMines
  }.get

  require(dimensions.area > 0)

  def nearbyMines(location: Location): Int = location.neighbors(dimensions).count(apply.andThen(Mine.eq))

  def apply(location: Location): Cell = mines(location.y)(location.x)

  def extracted(sector: Sector): Minefield = Minefield(
    mines
      .slice(sector.upperLeft.y, sector.lowerRight.y + 1)
      .map(_.slice(sector.upperLeft.x, sector.lowerRight.x + 1))
  )

  def combined(expand: Boolean, overwrite: Boolean)(start: Location)(other: Minefield): Minefield = {
    val otherLowerRight = Location(
      y = start.y + other.dimensions.height - 1,
      x = start.x + other.dimensions.width - 1
    )

    val combinedUpperLeft = Location(
      y = Math.min(0, start.y),
      x = Math.min(0, start.x)
    )

    val combinedLowerRight = Location(
      y = Math.max(dimensions.height - 1, otherLowerRight.y),
      x = Math.max(dimensions.width - 1, otherLowerRight.x)
    )

    val newDimensions = Dimensions(
      height = combinedLowerRight.y - combinedUpperLeft.y + 1,
      width = combinedLowerRight.x - combinedUpperLeft.x + 1
    )

    val resultingMinefield = Array.fill(newDimensions.height, newDimensions.width)(Empty)
    for (y <- 0 until dimensions.height; x <- 0 until dimensions.width)
      resultingMinefield(y - combinedUpperLeft.y)(x - combinedUpperLeft.x) = mines(y)(x)

    for (y <- 0 until other.dimensions.height; x <- 0 until other.dimensions.width) {
      if (overwrite || other.mines(y)(x) == Mine)
        resultingMinefield(start.y + y - combinedUpperLeft.y)(start.x + x - combinedUpperLeft.x) = other.mines(y)(x)
    }

    Minefield((
      if (!expand) {
        resultingMinefield.slice(
          Math.max(0, -combinedUpperLeft.y),
          Math.max(0, -combinedUpperLeft.y) + dimensions.height
        ).map(_.slice(
          Math.max(0, -combinedUpperLeft.x),
          Math.max(0, -combinedUpperLeft.x) + dimensions.width
        ))
      } else
        resultingMinefield
      )
      .map(_.toVector)
      .toVector
    )
  }

  override def toString: String = mines.map(_.map(_.toString).mkString).mkString(System.lineSeparator)
}

object Minefield {
  enum Cell(representation: Char) {
    case Empty extends Cell('-')
    case Mine extends Cell('#')

    override def toString: String = representation.toString
  }

  private val cellRepresentationMappings = Cell.values.map { cell => (cell.toString.head, cell) }.toMap

  def parse(minefield: Iterable[Iterable[Char]]): Try[Minefield] = {
    Try {
      Minefield(minefield.map(_.map { ch =>
        cellRepresentationMappings.getOrElse(ch, {
          throw InvalidCharacterException(ch)
        })
      }.toVector).toVector)
    }
  }

  def empty(dimensions: Dimensions): Minefield = Minefield(Vector.fill(dimensions.height, dimensions.width)(Empty))

  case class InvalidCharacterException(ch: Char) extends IllegalArgumentException(s"Invalid character: $ch")
}
