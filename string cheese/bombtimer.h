#pragma once




namespace ap::features::bombtimer {
//
//	void Misc::drawBombTimer() noexcept
//{
//    if (config.misc.bombTimer) {
//        for (int i = interfaces.engine->getMaxClients(); i <= interfaces.entityList->getHighestEntityIndex(); i++) {
//            Entity* entity = interfaces.entityList->getEntity(i);
//            if (!entity || entity->isDormant() || entity->getClientClass()->classId != ClassId::PlantedC4 || !entity->c4Ticking())
//                continue;
//
//            interfaces.surface->setTextFont(Surface::font);
//            interfaces.surface->setTextColor(250.0f, 0.0f, 0.0f, 255.0f);
//            auto drawPositionY{ interfaces.surface->getScreenSize().second / 2 };
//            interfaces.surface->setTextPosition(5, drawPositionY);
//            auto bombText{ (std::wstringstream{ } << L"Bomb on " << (!entity->c4BombSite() ? 'A' : 'B') << L" : " << std::setprecision(3) << (std::max)(entity->c4BlowTime() - memory.globalVars->currenttime, 0.0f) << L" s").str() };
//            drawPositionY += interfaces.surface->getTextSize(Surface::font, bombText.c_str()).second;
//            interfaces.surface->printText(bombText.c_str());
//            if (entity->c4Defuser() != -1) {
//                interfaces.surface->setTextColor(0.0f, 0.0f, 255.0f, 255.0f);
//                interfaces.surface->setTextPosition(5, drawPositionY);
//                static PlayerInfo playerInfo;
//                if (interfaces.engine->getPlayerInfo(interfaces.entityList->getEntityFromHandle(entity->c4Defuser())->index(), playerInfo)) {
//                    static wchar_t name[128];
//                    if (MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, -1, name, 128)) {
//                        interfaces.surface->printText((std::wstringstream{ } << name << L" is defusing: " << std::setprecision(4) << (std::max)(entity->c4DefuseCountDown() - memory.globalVars->currenttime, 0.0f) << L" s").str().c_str());
//                    }
//                }
//            }
//        }
//    }
//}
}