package com.djordjemilanovic.minesweeper
package utils

import scala.util.Random

case class Location(y: Int, x: Int) {
  override def toString = s"($y, $x)"

  def neighbors(dimensions: Dimensions): Seq[Location] =
    for {
      dy <- -1 to 1
      dx <- -1 to 1
      if dy != 0 || dx != 0
      neighbor = Location(y + dy, x + dx)
      if dimensions.contains(neighbor)
    } yield neighbor
    
  def +(that: Location): Location = Location(this.y + that.y, this.x + that.x)
}

object Location {
  def random(dimensions: Dimensions) = Location(
    Random.nextInt(dimensions.height),
    Random.nextInt(dimensions.width)
  )
  
  val origin = Location(0, 0)
}