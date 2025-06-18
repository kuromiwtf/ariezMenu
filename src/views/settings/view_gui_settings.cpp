#include "renderer/renderer.hpp"
#include "views/view.hpp"

namespace big
{
	void view::gui_settings()
	{
		components::sub_title("SETTINGS_UI_SCALE"_T);
		static auto gui_scale = g.window.gui_scale;
		ImGui::SliderFloat("##gui-scale", &gui_scale, 0.75f, 1.5f, "%.2f");
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			g.window.gui_scale = gui_scale;
			g_renderer.rescale(g.window.gui_scale);
		}

		components::sub_title("Header Image"_T);
		static std::vector<std::string> header_names = {"ariez (Purple)", "ariez (Grey)", "ariez (Rasta)", "ariez (Red)", "ariez (Blue)", "ariez (Batman 1)", "ariez(Batman 2)"};
		static std::vector<std::string> header_images = {"header1.png", "header2.png", "header3.png", "header4.png", "header5.png", "header6.png", "header7.png"};
		static int selected_header_index              = 0;

		// Define a struct to hold the theme colors
		struct ThemeColors
		{
			ImU32 background_color;
			ImU32 text_color;
			ImU32 button_color;
			ImU32 frame_color;
		};

		// Theme color definitions in ImU32 format (background, text, button, frame)
		static std::vector<ThemeColors> theme_colors = {
		    // Purple theme (default for header1.png)
		    {3717464384, 4294967295, 2936061451, 2936012800},  // Purple: Default Purple/Green
		    {3712889114, 4294967295, 2939960380, 4278190080},  // Grey: monochrome
		    {3707780114, 4294967295, 2936013055, 4279526240},  // Rasta: Dark Green/Red/Yellow
		    {3707764844, 4294967295, 2936061694, 4278190080},  // Red: Red/Orange/Black
		    {3723006720, 4294967295, 2936061694, 4278190080},  // Blue: Blue/Gold
		    {3707764736, 4294967295, 2939960380, 2944302718}, // Batman 1
		    {3707764736, 4294967295, 2939960380, 2944302718}  // Batman 2
		};


		// Checkbox for toggling themes
		if (ImGui::Checkbox("Preset Themes", &g.window.themes_enabled))
		{
			if (!g.window.themes_enabled)
			{
				// Revert to default colors defined in settings.hpp
				g.window.background_color = 3717464384;
				g.window.text_color       = 4294967295;
				g.window.button_color     = 2936061451;
				g.window.frame_color      = 2936012800;
			}
		}
		if (ImGui::IsItemHovered()) // Check if the button is hovered
		{
			ImGui::SetTooltip("Enable this to apply color schemes with the selected header image."); // Show the tooltip
		}

		// Display combo box for header selection
		if (ImGui::BeginCombo("Select Header Image", header_names[selected_header_index].c_str()))
		{
			for (int i = 0; i < header_names.size(); i++)
			{
				const bool is_selected = (selected_header_index == i);
				if (ImGui::Selectable(header_names[i].c_str(), is_selected))
				{
					selected_header_index                  = i;
					g.window.selected_header_image         = header_images[selected_header_index];
					g.window.selected_header_image_changed = true;

					// Apply theme colors if themes are enabled
					if (g.window.themes_enabled)
					{
						// Access the theme colors directly from the struct
						ThemeColors selected_theme = theme_colors[selected_header_index];
						
						g.window.background_color = selected_theme.background_color;
						g.window.text_color       = selected_theme.text_color;
						g.window.button_color     = selected_theme.button_color;
						g.window.frame_color      = selected_theme.frame_color;
					}
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (!g.window.themes_enabled)
		{
			components::sub_title("SETTINGS_UI_COLOR"_T);
			static ImVec4 col_gui = ImGui::ColorConvertU32ToFloat4(g.window.background_color);
			if (ImGui::ColorEdit4("SETTINGS_UI_COLOR_PICKER"_T.data(), (float*)&col_gui, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.window.background_color = ImGui::ColorConvertFloat4ToU32(col_gui);
			}

			static ImVec4 col_text = ImGui::ColorConvertU32ToFloat4(g.window.text_color);
			if (ImGui::ColorEdit4("VIEW_GUI_SETTINGS_TEXT_COLOR"_T.data(), (float*)&col_text, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.window.text_color = ImGui::ColorConvertFloat4ToU32(col_text);
			}

			static ImVec4 col_button = ImGui::ColorConvertU32ToFloat4(g.window.button_color);
			if (ImGui::ColorEdit4("VIEW_GUI_SETTINGS_BUTTON_COLOR"_T.data(), (float*)&col_button, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.window.button_color = ImGui::ColorConvertFloat4ToU32(col_button);
			}

			static ImVec4 col_frame = ImGui::ColorConvertU32ToFloat4(g.window.frame_color);
			if (ImGui::ColorEdit4("VIEW_GUI_SETTINGS_FRAME_COLOR"_T.data(), (float*)&col_frame, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoSidePreview))
			{
				g.window.frame_color = ImGui::ColorConvertFloat4ToU32(col_frame);
			}
		}

		components::sub_title("VIEW_GUI_SETTINGS_INGAME_OVERLAY"_T);
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_OVERLAY"_T.data(), &g.window.ingame_overlay.opened);
		ImGui::SameLine();
		ImGui::Checkbox("VIEW_GUI_SETTINGS_OVERLAY_SHOW_WHEN_MENU_OPENED"_T.data(), &g.window.ingame_overlay.show_with_menu_opened);

		ImGui::BeginGroup();

		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_FPS"_T.data(), &g.window.ingame_overlay.show_fps);
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_PLAYERS"_T.data(), &g.window.ingame_overlay.show_players);
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_TIME"_T.data(), &g.window.ingame_overlay.show_time);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("VIEW_GUI_SETTINGS_SHOW_TIME_TOOLTIP"_T.data());
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_INDICATORS"_T.data(), &g.window.ingame_overlay.show_indicators);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_REPLAY_INTERFACE"_T.data(), &g.window.ingame_overlay.show_replay_interface);
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_POSITION"_T.data(), &g.window.ingame_overlay.show_position);
		ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_GAME_VERSION"_T.data(), &g.window.ingame_overlay.show_game_versions);

		ImGui::EndGroup();

		ImGui::Checkbox("VIEW_GUI_FORMAT_MONEY"_T.data(), &g.window.gui.format_money);
		ImGui::SameLine();
		ImGui::Checkbox("METRIC_WARNING"_T.data(), &g.notifications.warn_metric);

		if (g.window.ingame_overlay.show_indicators)
		{
			if (ImGui::TreeNode("VIEW_GUI_SETTINGS_OVERLAY_INDICATORS"_T.data()))
			{
				ImGui::BeginGroup();
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_PLAYER_GODMODE"_T.data(), &g.window.ingame_overlay_indicators.show_player_godmode);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_OFF_THE_RADAR"_T.data(), &g.window.ingame_overlay_indicators.show_off_radar);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_VEHICLE_GODMODE"_T.data(), &g.window.ingame_overlay_indicators.show_vehicle_godmode);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_NEVER_WANTED"_T.data(), &g.window.ingame_overlay_indicators.show_never_wanted);
				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_INFINITE_AMMO"_T.data(), &g.window.ingame_overlay_indicators.show_infinite_ammo);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_ALWAYS_FULL_AMMO"_T.data(), &g.window.ingame_overlay_indicators.show_always_full_ammo);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_INFINITE_MAGAZINE"_T.data(), &g.window.ingame_overlay_indicators.show_infinite_mag);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_AIMBOT"_T.data(), &g.window.ingame_overlay_indicators.show_aimbot);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_TRIGGERBOT"_T.data(), &g.window.ingame_overlay_indicators.show_triggerbot);
				ImGui::Checkbox("VIEW_GUI_SETTINGS_SHOW_INVISIBILITY"_T.data(), &g.window.ingame_overlay_indicators.show_invisibility);
				ImGui::EndGroup();

				ImGui::TreePop();
			}
		}
	}

}
