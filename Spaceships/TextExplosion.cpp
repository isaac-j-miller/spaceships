#include "TextExplosion.h"
sf::Font TextExplosion::font;
sf::Color TextExplosion::fillColor;
TextExplosion::TextExplosion(const std::string& t, const point& topLeft, const float& fs){
	position = topLeft;
	text = t;
	fontSize = fs;
	duration = 200;
	sfText.setString("");
	sfText.setFont(font);
	sfText.setStyle(sf::Text::Bold);
	sfText.setFillColor(fillColor);
	sfText.setCharacterSize(fontSize);
	sfText.setString(text);
	sfText.setPosition(0, 0);
	boxSize.x = sfText.getLocalBounds().width;
	boxSize.y = sfText.getLocalBounds().height;
	position = position - boxSize*.5;
	setImage();
}
TextExplosion::~TextExplosion() {

}
void TextExplosion::Init(const std::string& fontFile, const sf::Color fc) {
	font.loadFromFile("pixel_font.ttf");
	fillColor = fc;
}
void TextExplosion::setImage() {
	sf::RenderTexture render;
	render.create(boxSize.x*1.5, boxSize.y*2);
	render.draw(sfText);
	render.display();
	iTexture = render.getTexture();
	sprite.setTexture(iTexture);
	sprite.setPosition(position.x, position.y);
}