package com.djordjemilanovic.minesweeper
package utils

case class Dimensions(height: Int, width: Int) {
  def area: Int = height * width
  def transposed: Dimensions = Dimensions(width, height)

  override def toString = s"($height X $width)"

  def contains(location: Location): Boolean =
    location.y >= 0 && location.y < height && location.x >= 0 && location.x < width
}