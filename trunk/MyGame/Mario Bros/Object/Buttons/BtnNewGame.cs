﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Mario_Bros.Framework;

namespace Mario_Bros.Object.Buttons
{
    class BtnNewGame : Buttons
    {
        public BtnNewGame(IDResource _ID, Vector2 _pos)
            : base(_ID, _pos)
        { }

        public override void Update()
        {
            if (IsFocus)
            {
                ID = IDResource.NEWGAME_Btn_Act;
            }
            else
                ID = IDResource.NEWGAME_Btn;
            Sprite = CResourceManager.GetInstance().GetResource(ID);
            base.Update();
        }

        public override void Draw(Microsoft.Xna.Framework.Graphics.SpriteBatch SB)
        {
            base.Draw(SB);
        }
    }
}
