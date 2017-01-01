import java.awt.Color;
import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;

import javax.imageio.ImageIO;

/**
 * A class that can be used to create a font atlas and a text file with data for the fontatla's layout.
 * 
 * The purpose of this class is to take a ttf file and create a fontatlas and data to use this fontatlas.
 * Intended to be used for font rendering in OpenGL or similar graphics application.
 * 
 * @author Felix Bärring
 */
public class FontCreator {

	/**
	 * A class that just holds data for the character data.
	 * 
	 * @author Felix Bärring
	 */
	static class CharData {

		public int width;
		public int height;

		public int xPosition;
		public int yPosition;

		@Override
		public String toString() {
			return "Width: " + width + ", Height: " + height + ", StoredX: " + xPosition + ", StoredY: " + yPosition;
		}

	}
	
	/**
	 * Creates a fontatlas and a data file for the layout of the fontatlas.
	 * 
	 * The two output files names will be placed in the root directory of where the program is ran.
	 * The name of the files will be 'outname'.png and 'outname'.txt .
	 * 
	 * @param font The font from which the fontatlas and layout data file will be created from
	 * @param antiAlias If anaialiasing should be used when creating the fontatlas 
	 * @param outName The name of the output files, .png and .txt will be prepended
	 */
	public static void createFontAtlasAndDataFile(int fontSize, java.awt.Font font, String outName) {

		final int CHAR_ARRAY_SIZE = 256;
		final CharData[] CHAR_ARRAY = new CharData[CHAR_ARRAY_SIZE];

		
		final int textureWidth = fontSize;
		final int textureHeight = fontSize;

		final BufferedImage fontAtlasImage = new BufferedImage(textureWidth, textureHeight, BufferedImage.TYPE_INT_ARGB);
		final Graphics2D fontGraphics = (Graphics2D) fontAtlasImage.getGraphics();
		
		fontGraphics.setFont(font);
		fontGraphics.setColor(new Color(255, 255, 255, 1)); // COLOE WHITE
		fontGraphics.fillRect(0, 0, textureWidth, textureHeight);

		int fontHeight = 0;
		int rowHeight = 0;
		int positionX = 0;
		int positionY = 0;

		for (char ch = 0; ch < 256; ch++) {

			BufferedImage fontImage = crateCharImage(ch, fontGraphics, font);
			CharData charData = new CharData();

			charData.width = fontImage.getWidth();
			charData.height = fontImage.getHeight();

			if (positionX + charData.width >= textureWidth) {
				positionX = 0;
				positionY += rowHeight;
				rowHeight = 0;
			}

			fontGraphics.drawImage(fontImage, positionX, positionY, null);
			
			charData.xPosition = positionX;
			charData.yPosition = positionY;

			if (charData.height > fontHeight) {
				fontHeight = charData.height;
			}

			if (charData.height > rowHeight) {
				rowHeight = charData.height;
			}

			positionX += charData.width;
			CHAR_ARRAY[ch] = charData;

		}

		try {
			
			// Write the font atlas
			ImageIO.write(fontAtlasImage, "PNG", new File(outName+".png"));
			
			// Write the font data
			PrintWriter writer = new PrintWriter(outName+".txt");
			writer.println("// Format: Width, Data, X, Y");

			for(CharData data : CHAR_ARRAY ) {
				writer.println(data.width + " " + data.height + " " + data.xPosition + " " + data.yPosition);
			}

			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	/**
	 * A helper function that creates a BufferedImage of a char. 
	 * The Buffered Image can the be used to render inside the fontatlas. 
	 * 
	 * @param ch The charachter to render.
	 * @param graphics The graphics context of the fontatlas / font
	 * @param font The font that the char will be rendered with
	 * @return A BufferedImage that represents the char
	 */
	private static BufferedImage crateCharImage(char ch, Graphics2D graphics, java.awt.Font font) {

		FontMetrics fontMetrics = graphics.getFontMetrics();
		int charwidth = fontMetrics.charWidth(ch);

		// char width < 0 is an error, just set it to 1
		if (charwidth <= 0) {
			charwidth = 1;
		}

		int charheight = fontMetrics.getHeight();

		BufferedImage fontImage = new BufferedImage(charwidth, charheight, BufferedImage.TYPE_INT_ARGB);
		Graphics2D charGraphics = (Graphics2D) fontImage.getGraphics();

		charGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

		charGraphics.setFont(font);
		charGraphics.setColor(Color.WHITE);
		charGraphics.drawString(String.valueOf(ch), 0, 0 + fontMetrics.getAscent());
		return fontImage;

	}

	/**
	 * The Main function of the program.
	 * Creates a fontatlas and a data file for the layout of the fontatlas.
	 * 
	 * @param argv Must contain three Strings, the firest one is the desired size of the texture that shall be 
     *        generated. The second is the name of the ttf file that should be in the same root directory where this 
     *        program is ran. The third String is the name of the output files, .png and .txt will be appended on the 
     *        file names.
	 */
	public static void main(String[] argv) {

		if (argv.length < 2) {
			System.out.println("Error, need to provide the following arguments: Desired texture dimension, name of" + 
                "the ttf file and the name of the output files.");
			System.exit(0);
		}

		final int size = Integer.parseInt(argv[0]);
		String ttfName = argv[1];
		String outName = argv[2];

		Font font;
		try {
			
			font = Font.createFont(Font.TRUETYPE_FONT, new FileInputStream(ttfName));
			font = font.deriveFont((float) 60);
			font = font.deriveFont(Font.PLAIN);
			FontCreator.createFontAtlasAndDataFile(size, font, outName);
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (FontFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}


}
