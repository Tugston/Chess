#pragma once


namespace GraphicsEngine {

	class BasePiece {
	public:
		//BasePiece();
		//~BasePiece() {};

	protected:
		virtual void Selected() = 0;
		virtual void DeSelected() = 0;
		virtual void Hovered() {};

	protected:

		unsigned int textureID;
		
		//may template later, possibly never, some games need screen position for location, others need cell locations
		//I just dont feel like going that in depth, for something I plan on making chess and checkers with
		//so all piece location stuff can just go in the specific game base piece class
	};

}