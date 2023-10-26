using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Chess
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        GraphicsDevice device;
        SpriteBatch spriteBatch;
        Texture2D piecesPic;
        Texture2D boardPic;
        Texture2D buttonsPic;
        Texture2D mousePic;
        Viewport viewport;
        ChessBoard board;
        Button butBack;
        Button butNext;
        Button butPlay;
        MouseState mLast;
        MouseState mNow;
        Vector2 mousePos;

        int[,,] boards;
        int latestBoard;
        int viewedBoard;
        bool wrap;
        bool turn;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            graphics.PreferredBackBufferWidth = 1680;
            graphics.PreferredBackBufferHeight = 1050;
            graphics.PreferMultiSampling = false;
            graphics.IsFullScreen = false;
            IsMouseVisible = false;

            Content.RootDirectory = "Content";

            mousePos = new Vector2(0, 0);
            board = new ChessBoard(true, true);
            butBack = new Button(new Rectangle(0, 0, 78, 124), 1, false);
            butNext = new Button(new Rectangle(78, 0, 78, 124), 2, false);
            butPlay = new Button(new Rectangle(156, 0, 140, 124), 3, true);

            // initialize the boards array
            boards = new int[10, 8, 8];
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    boards[0, i, j] = board.read(i, j);
                }
            }
            for (int h = 1; h < 10; h++)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        boards[h, i, j] = 0;
                    }
                }
            }

            // initialize other game state variables
            latestBoard = 0;
            viewedBoard = 0;
            wrap = false;
            turn = true;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            device = GraphicsDevice;
            spriteBatch = new SpriteBatch(GraphicsDevice);
            piecesPic = Content.Load<Texture2D>("Pieces");
            boardPic = Content.Load<Texture2D>("Board");
            buttonsPic = Content.Load<Texture2D>("Buttons");
            mousePic = Content.Load<Texture2D>("Mouse");
            viewport = graphics.GraphicsDevice.Viewport;

            butBack.setTexture(buttonsPic);
            butBack.initEffects(device);
            butNext.setTexture(buttonsPic);
            butNext.initEffects(device);
            butPlay.setTexture(buttonsPic);
            butPlay.initEffects(device);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            mLast = mNow;
            mNow = Mouse.GetState();
            mousePos.X = mNow.X;
            mousePos.Y = mNow.Y;

            butBack.update(mNow, mLast);
            butNext.update(mNow, mLast);
            butPlay.update(mNow, mLast);

            if (butBack.getExecute())
            {
                if (viewedBoard == 0) viewedBoard = 9;
                else viewedBoard--;
                if ((viewedBoard == 0 && (!wrap || latestBoard == 9)) || (viewedBoard == (latestBoard + 1))) butBack.turnOff();
                butNext.turnOn();
                butPlay.turnOff();
            }

            if (butNext.getExecute())
            {
                if (viewedBoard == 9) viewedBoard = 0;
                else viewedBoard++;
                butBack.turnOn();
                if (viewedBoard == latestBoard)
                {
                    butNext.turnOff();
                    butPlay.turnOn();
                }
            }

            if (butPlay.getExecute())
            {
                // execute turn
                if (turn) board.doTurn(true, false, 1);
                else board.doTurn(false, false, 1);
                // change game state variables
                if (latestBoard == 9)
                {
                    wrap = true;  // make boards wrap around from 9 to 0
                    latestBoard = 0;
                }
                else latestBoard++;
                viewedBoard = latestBoard;
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        boards[latestBoard, i, j] = board.read(i, j);
                    }
                }
                butBack.turnOn();
                turn = !turn;
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            graphics.GraphicsDevice.Clear(Color.DarkRed);

            // position images
            Vector2 boardLoc = new Vector2((viewport.Width - boardPic.Width) / 2, (viewport.Height - boardPic.Height) / 2 - 50);
            butBack.setLocation(viewport.Width / 2 - 200, (viewport.Height + boardPic.Height) / 2 + 10);
            butNext.setLocation(viewport.Width / 2 - 100, (viewport.Height + boardPic.Height) / 2 + 10);
            butPlay.setLocation(viewport.Width / 2 + 50, (viewport.Height + boardPic.Height) / 2 + 10);

            spriteBatch.Begin(blendState: BlendState.AlphaBlend);
            // Draw the chess board
            spriteBatch.Draw(boardPic, boardLoc, null, Color.White);

            // Draw the pieces
            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    if (boards[viewedBoard, x, y] != 0)
                    {
                        Rectangle sourceRect;
                        if (boards[viewedBoard, x, y] > 0) sourceRect = new Rectangle((boards[viewedBoard, x, y] - 1) * 64, 0, 64, 64);
                        else sourceRect = new Rectangle((-boards[viewedBoard, x, y] - 1) * 64, 64, 64, 64);
                        Vector2 piecePos = new Vector2(16 + x * 64, 16 + (7 - y) * 64);
                        spriteBatch.Draw(piecesPic, boardLoc + piecePos, sourceRect, Color.White);
                    }
                }
            }

            // Draw the buttons
            butBack.draw(spriteBatch);
            butNext.draw(spriteBatch);
            butPlay.draw(spriteBatch);

            spriteBatch.End();

            butBack.drawEffect(device);
            butNext.drawEffect(device);
            butPlay.drawEffect(device);

            spriteBatch.Begin(blendState: BlendState.AlphaBlend);

            // Draw the mouse
            spriteBatch.Draw(mousePic, mousePos, null, Color.White);

            spriteBatch.End();

            base.Draw(gameTime);
        }

    }
}
