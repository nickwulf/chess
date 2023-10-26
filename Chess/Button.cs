using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Chess {
    class Button {
        private Texture2D buttonsPic;
        private Rectangle sourceRect;
        private Vector2 loc;
        private int id;
        private bool active;
        private bool mouseOver;
        private bool pressed;
        private bool execute;
        private int timeOn;

        private Matrix worldMatrix;
        private BasicEffect basicEffect;
        private VertexPositionColor[] pointList;
        private VertexBuffer vertexBuffer;
        private short[] lineListIndices;
        
        public Button() {
            this.buttonsPic = null;
            this.sourceRect = new Rectangle(); ;
            this.loc = new Vector2();
            this.id = 0;
            this.active = false;
            mouseOver = false;
            pressed = false;
            execute = false;
            timeOn = 0;
        }

        public Button(Rectangle sourceRect, int id, bool active) {
            this.buttonsPic = null;
            this.sourceRect = sourceRect;
            this.loc = new Vector2();
            this.id = id;
            this.active = active;
            mouseOver = false;
            pressed = false;
            execute = false;
            timeOn = 0;
        }

        public void draw(SpriteBatch batch) {
            // If button has an image associated with it then draw it
            if (buttonsPic != null) {
                if (pressed && mouseOver && active) batch.Draw(buttonsPic, loc, sourceRect, new Color(179, 179, 179));
                else batch.Draw(buttonsPic, loc, sourceRect, Color.White);
            }
        }

        public void update(MouseState mNow, MouseState mLast) {
            execute = false;
            mouseOver = false;
            if (active) {
                timeOn++;
                if (timeOn >= 120) timeOn = 0;

                if (id == 1 &&
                    mNow.X >= loc.X && mNow.X <= (loc.X + sourceRect.Width) &&
                    mNow.Y >= loc.Y && mNow.Y <= (loc.Y + sourceRect.Height) &&
                    (mNow.Y - loc.Y) >= (-(mNow.X - loc.X) + 46) &&
                    (mNow.Y - loc.Y) <= ((mNow.X - loc.X) + 77)) mouseOver = true;
                if (id == 2 &&
                    mNow.X >= loc.X && mNow.X <= (loc.X + sourceRect.Width) &&
                    mNow.Y >= loc.Y && mNow.Y <= (loc.Y + sourceRect.Height) &&
                    (mNow.Y-loc.Y) >= ((mNow.X-loc.X) - 31) &&
                    (mNow.Y-loc.Y) <= (-(mNow.X-loc.X) + 154)) mouseOver = true;
                if (id == 3 &&
                    mNow.X >= loc.X && mNow.X <= (loc.X + sourceRect.Width) &&
                    mNow.Y >= loc.Y && mNow.Y <= (loc.Y + sourceRect.Height)) mouseOver = true;

                if (mouseOver && mNow.LeftButton == ButtonState.Pressed && mLast.LeftButton == ButtonState.Released) pressed = true;
                if (pressed && mNow.LeftButton == ButtonState.Released && mLast.LeftButton == ButtonState.Pressed) {
                    pressed = false;
                    if (mouseOver) {
                        timeOn = 0;
                        execute = true;
                    }
                }
            }
        }

        public void turnOn() {
            active = true;
        }

        public void turnOff() {
            active = false;
            pressed = false;
            execute = false;
            timeOn = 0;
        }

        public void setTexture(Texture2D buttonsPic) {
            this.buttonsPic = buttonsPic;
        }

        public void setLocation(int locX, int locY) {
            loc = new Vector2(locX, locY);
            worldMatrix = Matrix.CreateTranslation(locX, locY, 0);
        }

        public bool getExecute() {
            return execute;
        }

        public void initEffects(GraphicsDevice device) {
            Matrix viewMatrix = Matrix.CreateLookAt(new Vector3(0.0f, 0.0f, 1.0f), Vector3.Zero, Vector3.Up);
            Matrix projectionMatrix = Matrix.CreateOrthographicOffCenter(0, (float)device.Viewport.Width, (float)device.Viewport.Height, 0, 1.0f, 1000.0f);
            basicEffect = new BasicEffect(device);
            basicEffect.VertexColorEnabled = true;
            worldMatrix = Matrix.CreateTranslation(0, 0, 0);
            basicEffect.World = worldMatrix;
            basicEffect.View = viewMatrix;
            basicEffect.Projection = projectionMatrix;
            if (id == 1) {
                pointList = new VertexPositionColor[9];
                pointList[0] = new VertexPositionColor(new Vector3(54, 30, 0), Color.White);
                pointList[1] = new VertexPositionColor(new Vector3(54, 93, 0), Color.White);
                pointList[2] = new VertexPositionColor(new Vector3(22, 61, 0), Color.White);
                pointList[3] = new VertexPositionColor(new Vector3(23, 61, 0), Color.White);
                pointList[4] = new VertexPositionColor(new Vector3(53, 31, 0), Color.White);
                pointList[5] = new VertexPositionColor(new Vector3(53, 91, 0), Color.White);
                pointList[6] = new VertexPositionColor(new Vector3(23, 61, 0), Color.White);
                pointList[7] = new VertexPositionColor(new Vector3(24, 61, 0), Color.White);
                pointList[8] = new VertexPositionColor(new Vector3(53, 32, 0), Color.White);
                lineListIndices = new short[12] { 0,1, 1,2, 3,4, 4,5, 5,6, 7,8 };
            }
            if (id == 2) {
                pointList = new VertexPositionColor[12];
                pointList[0] = new VertexPositionColor(new Vector3(23, 30, 0), Color.White);
                pointList[1] = new VertexPositionColor(new Vector3(23, 94, 0), Color.White);
                pointList[2] = new VertexPositionColor(new Vector3(24, 31, 0), Color.White);
                pointList[3] = new VertexPositionColor(new Vector3(24, 93, 0), Color.White);
                pointList[4] = new VertexPositionColor(new Vector3(25, 32, 0), Color.White);
                pointList[5] = new VertexPositionColor(new Vector3(55, 62, 0), Color.White);
                pointList[6] = new VertexPositionColor(new Vector3(25, 33, 0), Color.White);
                pointList[7] = new VertexPositionColor(new Vector3(54, 62, 0), Color.White);
                pointList[8] = new VertexPositionColor(new Vector3(54, 62, 0), Color.White);
                pointList[9] = new VertexPositionColor(new Vector3(24, 92, 0), Color.White);
                pointList[10] = new VertexPositionColor(new Vector3(53, 62, 0), Color.White);
                pointList[11] = new VertexPositionColor(new Vector3(24, 91, 0), Color.White);
                lineListIndices = new short[12] { 0,1, 2,3, 4,5, 6,7, 8,9, 10,11 };
            }
            if (id == 3) {
                pointList = new VertexPositionColor[24];
                pointList[0] = new VertexPositionColor(new Vector3(35, 30, 0), Color.White);
                pointList[1] = new VertexPositionColor(new Vector3(35, 94, 0), Color.White);
                pointList[2] = new VertexPositionColor(new Vector3(36, 31, 0), Color.White);
                pointList[3] = new VertexPositionColor(new Vector3(36, 93, 0), Color.White);
                pointList[4] = new VertexPositionColor(new Vector3(37, 32, 0), Color.White);
                pointList[5] = new VertexPositionColor(new Vector3(67, 62, 0), Color.White);
                pointList[6] = new VertexPositionColor(new Vector3(37, 33, 0), Color.White);
                pointList[7] = new VertexPositionColor(new Vector3(66, 62, 0), Color.White);
                pointList[8] = new VertexPositionColor(new Vector3(66, 62, 0), Color.White);
                pointList[9] = new VertexPositionColor(new Vector3(36, 92, 0), Color.White);
                pointList[10] = new VertexPositionColor(new Vector3(65, 62, 0), Color.White);
                pointList[11] = new VertexPositionColor(new Vector3(36, 91, 0), Color.White);

                pointList[12] = new VertexPositionColor(new Vector3(78, 30, 0), Color.White);
                pointList[13] = new VertexPositionColor(new Vector3(78, 94, 0), Color.White);
                pointList[14] = new VertexPositionColor(new Vector3(79, 31, 0), Color.White);
                pointList[15] = new VertexPositionColor(new Vector3(79, 93, 0), Color.White);
                pointList[16] = new VertexPositionColor(new Vector3(80, 32, 0), Color.White);
                pointList[17] = new VertexPositionColor(new Vector3(110, 62, 0), Color.White);
                pointList[18] = new VertexPositionColor(new Vector3(80, 33, 0), Color.White);
                pointList[19] = new VertexPositionColor(new Vector3(109, 62, 0), Color.White);
                pointList[20] = new VertexPositionColor(new Vector3(109, 62, 0), Color.White);
                pointList[21] = new VertexPositionColor(new Vector3(79, 92, 0), Color.White);
                pointList[22] = new VertexPositionColor(new Vector3(108, 62, 0), Color.White);
                pointList[23] = new VertexPositionColor(new Vector3(79, 91, 0), Color.White);
                lineListIndices = new short[24] { 0,1, 2,3, 4,5, 6,7, 8,9, 10,11, 12,13, 14,15, 16,17, 18,19, 20,21, 22,23 };
            }

            vertexBuffer = new VertexBuffer(device, typeof(VertexPositionColor), pointList.Length, BufferUsage.None);
            vertexBuffer.SetData<VertexPositionColor>(pointList);

        }

        public void drawEffect(GraphicsDevice device) {
            //If button has an effect associated with it then draw it
            //VertexDeclaration vertexDeclaration = new VertexDeclaration(device, VertexPositionColor.VertexElements);
            //device.VertexDeclaration = vertexDeclaration;
            double colorMag = Math.Abs(Math.Sin(timeOn * Math.PI / 120));
            if (pressed && mouseOver && active) for (int i = 0; i < pointList.Length; i++) pointList[i].Color = new Color(255, 128, 0, 255);
            else for (int i = 0; i < pointList.Length; i++) pointList[i].Color = new Color((int)(64 + colorMag*191), (int)(64 + colorMag*64), (int)(64 - 64*colorMag), 255);
            basicEffect.World = worldMatrix;
            //basicEffect.Begin();
            foreach (EffectPass pass in basicEffect.CurrentTechnique.Passes) {
                pass.Apply();
                if (id == 1) device.DrawUserIndexedPrimitives<VertexPositionColor>(PrimitiveType.LineList, pointList, 0, pointList.Length, lineListIndices, 0, 6);
                if (id == 2) device.DrawUserIndexedPrimitives<VertexPositionColor>(PrimitiveType.LineList, pointList, 0, pointList.Length, lineListIndices, 0, 6);
                if (id == 3) device.DrawUserIndexedPrimitives<VertexPositionColor>(PrimitiveType.LineList, pointList, 0, pointList.Length, lineListIndices, 0, 12);
                //pass.End();
            }
            //basicEffect.End();
        }
    }
}
