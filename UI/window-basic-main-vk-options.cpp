/******************************************************************************
    Copyright (C) 2016 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include <vk-api/methods.h>

#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"

static size_t gamesCategoryId;

static std::string getUsername()
{
	json_t *root = vk_get_user(vk_access_token.c_str());
	json_t *user;
	std::string name;

	user = json_array_get(json_object_get(root, "response"), 0);
	if (!user) {
		name = "";
	} else {
		name = JSON_GET_STRING(user, "first_name");
		name += ' ';
		name += JSON_GET_STRING(user, "last_name");
	}

	json_decref(root);
	return name;
}

static json_t *getUserOwnedGroups()
{
	json_t *root = vk_get_user_owned_groups(vk_access_token.c_str());

	json_t *groupsArray = json_deep_copy(
		json_object_get(json_object_get(root, "response"), "items"));

	json_decref(root);
	return groupsArray;
}

void OBSBasic::LoadVKStreamTargets()
{
	std::string username;
	json_t *groups, *item;
	size_t i;

	username = getUsername();
	ui->VKStreamTargets->addItem(QT_UTF8(username.c_str()), "");

	groups = getUserOwnedGroups();
	json_array_foreach(groups, i, item) {
		const char *name = JSON_GET_STRING(item, "name");
		size_t id = JSON_GET_INTEGER(item, "id");

		ui->VKStreamTargets->addItem(QT_UTF8(name), id);
	}
	json_decref(groups);
}

void OBSBasic::toggleGamesVisible(int index)
{
	if (ui->VKStreamCategories->itemData(index) == gamesCategoryId) {
		ui->VKStreamGames->setVisible(true);
		ui->vkGamesLabel->setVisible(true);
	} else {
		ui->VKStreamGames->setVisible(false);
		ui->vkGamesLabel->setVisible(false);
	}
}

static json_t *getStreamCategories()
{
	json_t *root = vk_get_stream_categories(vk_access_token.c_str());

	json_t *categories = json_deep_copy(json_object_get(root, "response"));

	json_decref(root);
	return categories;
}

void OBSBasic::LoadVKStreamCategories()
{
	json_t *categories, *games, *category, *game;
	const char *catLabel, *gameLabel;
	size_t i, j, catId, gameId;

	categories = getStreamCategories();

	json_array_foreach(categories, i, category) {
		catLabel = JSON_GET_STRING(category, "label");
		catId = JSON_GET_INTEGER(category, "id");
		ui->VKStreamCategories->addItem(QT_UTF8(catLabel), catId);

		games = json_object_get(category, "sublist");
		if (!games)
			continue;

		json_array_foreach(games, j, game) {
			gameLabel = JSON_GET_STRING(game, "label");
			gameId = JSON_GET_INTEGER(game, "id");
			ui->VKStreamGames->addItem(
					QT_UTF8(gameLabel), gameId);
		}
		gamesCategoryId = catId;
		QObject::connect(ui->VKStreamCategories,
				SIGNAL(currentIndexChanged(int)),
				this,
				SLOT(toggleGamesVisible(int)));
		toggleGamesVisible(ui->VKStreamCategories->currentIndex());
	}
	json_decref(categories);
}