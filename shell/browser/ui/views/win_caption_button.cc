// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "shell/browser/ui/views/win_caption_button.h"

#include "base/numerics/safe_conversions.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/frame/window_frame_util.h"
//#include "chrome/browser/ui/views/frame/browser_view.h"

#include "shell/browser/ui/views/win_frame_view.h"

#include "shell/common/color_util.h"  //FIXME(@mlaurencin): For obtaining default colors

#include "chrome/grit/theme_resources.h"
#include "ui/base/theme_provider.h"
#include "ui/gfx/animation/tween.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/geometry/rect_conversions.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/views/metadata/metadata_impl_macros.h"

namespace electron {

WinCaptionButton::WinCaptionButton(PressedCallback callback,
                                   WinFrameView* frame_view,
                                   ViewID button_type,
                                   const std::u16string& accessible_name)
    : views::Button(std::move(callback)),
      frame_view_(frame_view),
      button_type_(button_type) {
  SetAnimateOnStateChange(true);
  // Not focusable by default, only for accessibility.
  SetFocusBehavior(FocusBehavior::ACCESSIBLE_ONLY);
  SetAccessibleName(accessible_name);
}

gfx::Size WinCaptionButton::CalculatePreferredSize() const {
  // TODO(bsep): The sizes in this function are for 1x device scale and don't
  // match Windows button sizes at hidpi. <--TODO(mlaurencin): Due to this
  // comment, I think that the if condition might not apply to our case. Or it
  // may need a different conditional check
  int height = WindowFrameUtil::kWindows10GlassCaptionButtonHeightRestored;
  int base_width = WindowFrameUtil::kWindows10GlassCaptionButtonWidth;
  return gfx::Size(base_width + GetBetweenButtonSpacing(), height);
}

SkColor WinCaptionButton::GetBaseColor() const {
  // TODO(@mlaurencin): For handling light vs dark themed background colors in
  // future: Get the theme's calculated custom control button background color
  // (as it takes into account images, etc).  If none is specified (likely when
  // there is no theme active), fall back to the titlebar color.

  auto color = ToRGBHex(color_utils::GetSysSkColor(COLOR_BTNTEXT));

  if (button_type_ == VIEW_ID_MINIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::GetBaseColor() - VIEW_ID_MINIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_MAXIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::GetBaseColor() - VIEW_ID_MAXIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_RESTORE_BUTTON)
    LOG(INFO) << "WinCaptionButton::GetBaseColor() - VIEW_ID_RESTORE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_CLOSE_BUTTON)
    LOG(INFO) << "WinCaptionButton::GetBaseColor() - VIEW_ID_CLOSE_BUTTON - "
              << __LINE__;

  return color_utils::GetSysSkColor(COLOR_BTNTEXT);
}

void WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) {
  // FIXME(@mlaurencin): Remove after testing
  // LOG(INFO) << "WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) -
  // CALLED - " << __LINE__;
  if (button_type_ == VIEW_ID_MINIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) - "
                 "VIEW_ID_MINIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_MAXIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) - "
                 "VIEW_ID_MAXIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_RESTORE_BUTTON)
    LOG(INFO) << "WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) - "
                 "VIEW_ID_RESTORE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_CLOSE_BUTTON)
    LOG(INFO) << "WinCaptionButton::OnPaintBackground(gfx::Canvas* canvas) - "
                 "VIEW_ID_CLOSE_BUTTON - "
              << __LINE__;
  // Paint the background of the button (the semi-transparent rectangle that
  // appears when you hover or press the button).
  // FIXME(@mlaurencin): theme_provider is running as a nullptr, so determine
  // fix before uncommenting
  // const ui::ThemeProvider* theme_provider = GetThemeProvider();
  // LOG(INFO) << "WinCaptionButton::OnPaintBackground - " << (theme_provider ==
  // nullptr) << " - " << __LINE__; const SkColor bg_color =
  // theme_provider->GetColor(
  //     ThemeProperties::COLOR_CONTROL_BUTTON_BACKGROUND);

  // const SkColor bg_color =
  //     SkColorSetRGB(0xD3, 0xD3, 0xD3);  // FIXME(@mlaurencin): Temporary -
  //                                       // Remove once theme_provider is
  //                                       fixed
  // const SkColor bg_color =
  //     SkColorSetRGB(0x00, 0x00, 0x00);  // FIXME(@mlaurencin): Could possibly
  //     use to make button "transparent" and use banner as it's "color"

  const SkColor bg_color = color_utils::GetSysSkColor(
      COLOR_BTNFACE);  // FIXME(@mlaurencin): I think correct default color, but
                       // need to figure out proper active/non-active opacity

  // const SkAlpha theme_alpha = SkColorGetA(bg_color);
  // const SkAlpha theme_alpha = 0x00;  // FIXME(@mlaurencin): Could possibly
  // use to make button "transparent" and use banner as it's "color"
  const SkAlpha theme_alpha = 0xFF;

  gfx::Rect bounds = GetContentsBounds();
  // bounds.Inset(GetBetweenButtonSpacing(), 0, 0, 0);
  bounds.Inset(0, 0, 0, 0);  // TODO(@mlaurencin): How to make the buttons all
                             // right next to each other

  // FIXME(@mlaurencin): Remove temp and have the code below not cause a
  // linking error instead
  /*
  lld-link: error: undefined symbol: public: static unsigned char __cdecl
WindowFrameUtil::CalculateWindows10GlassCaptionButtonBackgroundAlpha(unsigned
char)
>>> referenced by
.\..\..\electron\shell\browser\ui\views\win_caption_button.cc:91
>>>               obj/electron/electron_lib/win_caption_button.obj:(public:
virtual void __cdecl electron::WinCaptionButton::OnPaintBackground(class
gfx::Canvas *))
  */

  canvas->FillRect(bounds, SkColorSetA(bg_color, theme_alpha));

  // auto temp = (theme_alpha == SK_AlphaOPAQUE ? 0xCC : theme_alpha);
  // if (theme_alpha > 0) {
  //   canvas->FillRect(bounds, SkColorSetA(bg_color, temp));
  // }

  // if (theme_alpha > 0) {
  //   canvas->FillRect(
  //       bounds,
  //       SkColorSetA(bg_color,
  //                   WindowFrameUtil::
  //                       CalculateWindows10GlassCaptionButtonBackgroundAlpha(
  //                           theme_alpha)));
  // }

  // FIXME(@mlaurencin): Temporary - Remove once theme_provider is fixed
  // if (theme_provider->HasCustomImage(IDR_THEME_WINDOW_CONTROL_BACKGROUND)) {
  //   // Figure out what portion of the background image to display
  //   const int button_display_order = GetButtonDisplayOrderIndex();
  //   const int base_button_width =
  //       WindowFrameUtil::kWindows10GlassCaptionButtonWidth;
  //   const int base_visual_spacing =
  //       WindowFrameUtil::kWindows10GlassCaptionButtonVisualSpacing;
  //   const int src_x =
  //       button_display_order * (base_button_width + base_visual_spacing);
  //   const int src_y = 0;

  //   canvas->TileImageInt(
  //       *theme_provider->GetImageSkiaNamed(IDR_THEME_WINDOW_CONTROL_BACKGROUND),
  //       src_x, src_y, bounds.x(), bounds.y(), bounds.width(),
  //       bounds.height());
  // }

  SkColor base_color;
  SkAlpha hovered_alpha, pressed_alpha;
  if (button_type_ == VIEW_ID_CLOSE_BUTTON) {
    base_color = SkColorSetRGB(0xE8, 0x11, 0x23);
    hovered_alpha = SK_AlphaOPAQUE;
    pressed_alpha = 0x98;
  } else {
    // Match the native buttons.
    base_color = GetBaseColor();
    hovered_alpha = 0x1A;
    pressed_alpha = 0x33;

    if (theme_alpha > 0) {
      // Theme buttons have slightly increased opacity to make them stand out
      // against a visually-busy frame image.
      constexpr float kAlphaScale = 1.3f;
      hovered_alpha = base::ClampRound<SkAlpha>(hovered_alpha * kAlphaScale);
      pressed_alpha = base::ClampRound<SkAlpha>(pressed_alpha * kAlphaScale);
    }
  }

  SkAlpha alpha;
  if (GetState() == STATE_PRESSED)
    alpha = pressed_alpha;
  else
    alpha = gfx::Tween::IntValueBetween(hover_animation().GetCurrentValue(),
                                        SK_AlphaTRANSPARENT, hovered_alpha);
  canvas->FillRect(bounds, SkColorSetA(base_color, alpha));
}

void WinCaptionButton::PaintButtonContents(gfx::Canvas* canvas) {
  PaintSymbol(canvas);
}

int WinCaptionButton::GetBetweenButtonSpacing() const {
  const int display_order_index = GetButtonDisplayOrderIndex();
  return display_order_index == 0
             ? 0
             : WindowFrameUtil::kWindows10GlassCaptionButtonVisualSpacing;
}

int WinCaptionButton::GetButtonDisplayOrderIndex() const {
  int button_display_order = 0;
  switch (button_type_) {
    case VIEW_ID_MINIMIZE_BUTTON:
      button_display_order = 0;
      break;
    case VIEW_ID_MAXIMIZE_BUTTON:
    case VIEW_ID_RESTORE_BUTTON:
      button_display_order = 1;
      break;
    case VIEW_ID_CLOSE_BUTTON:
      button_display_order = 2;
      break;
    default:
      NOTREACHED();
      return 0;
  }

  // Reverse the ordering if we're in RTL mode
  if (base::i18n::IsRTL())
    button_display_order = 2 - button_display_order;

  return button_display_order;
}

namespace {

// Canvas::DrawRect's stroke can bleed out of |rect|'s bounds, so this draws a
// rectangle inset such that the result is constrained to |rect|'s size.
void DrawRect(gfx::Canvas* canvas,
              const gfx::Rect& rect,
              const cc::PaintFlags& flags) {
  gfx::RectF rect_f(rect);
  float stroke_half_width = flags.getStrokeWidth() / 2;
  rect_f.Inset(stroke_half_width, stroke_half_width);
  canvas->DrawRect(rect_f, flags);
}

}  // namespace

void WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) {
  // FIXME(@mlaurencin): Remove after testing
  // LOG(INFO) << "WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) - CALLED -
  // " << __LINE__;
  if (button_type_ == VIEW_ID_MINIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) - "
                 "VIEW_ID_MINIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_MAXIMIZE_BUTTON)
    LOG(INFO) << "WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) - "
                 "VIEW_ID_MAXIMIZE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_RESTORE_BUTTON)
    LOG(INFO) << "WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) - "
                 "VIEW_ID_RESTORE_BUTTON - "
              << __LINE__;
  else if (button_type_ == VIEW_ID_CLOSE_BUTTON)
    LOG(INFO) << "WinCaptionButton::PaintSymbol(gfx::Canvas* canvas) - "
                 "VIEW_ID_CLOSE_BUTTON - "
              << __LINE__;

  SkColor symbol_color = GetBaseColor();
  // if (!GetEnabled() ||
  //     (!frame_view_->ShouldPaintAsActive() && GetState() != STATE_HOVERED &&
  //      GetState() != STATE_PRESSED)) {
  //   symbol_color =
  //       SkColorSetA(symbol_color,
  //       WinFrameView::kInactiveTitlebarFeatureAlpha);
  // } else if (button_type_ == VIEW_ID_CLOSE_BUTTON &&
  //            hover_animation().is_animating()) {
  //   symbol_color = gfx::Tween::ColorValueBetween(
  //       hover_animation().GetCurrentValue(), symbol_color, SK_ColorWHITE);
  // } else if (button_type_ == VIEW_ID_CLOSE_BUTTON &&
  //            (GetState() == STATE_HOVERED || GetState() == STATE_PRESSED)) {
  //   symbol_color = SK_ColorWHITE;
  // }

  if (button_type_ == VIEW_ID_CLOSE_BUTTON &&
      hover_animation().is_animating()) {
    symbol_color = gfx::Tween::ColorValueBetween(
        hover_animation().GetCurrentValue(), symbol_color, SK_ColorWHITE);
  } else if (button_type_ == VIEW_ID_CLOSE_BUTTON &&
             (GetState() == STATE_HOVERED || GetState() == STATE_PRESSED)) {
    symbol_color = SK_ColorWHITE;
  }

  gfx::ScopedCanvas scoped_canvas(canvas);
  const float scale = canvas->UndoDeviceScaleFactor();

  const int symbol_size_pixels = std::round(10 * scale);
  gfx::RectF bounds_rect(GetContentsBounds());
  bounds_rect.Scale(scale);
  gfx::Rect symbol_rect(gfx::ToEnclosingRect(bounds_rect));
  symbol_rect.ClampToCenteredSize(
      gfx::Size(symbol_size_pixels, symbol_size_pixels));

  cc::PaintFlags flags;
  flags.setAntiAlias(false);
  flags.setColor(symbol_color);
  flags.setStyle(cc::PaintFlags::kStroke_Style);
  // Stroke width jumps up a pixel every time we reach a new integral scale.
  const int stroke_width = std::floor(scale);
  flags.setStrokeWidth(stroke_width);

  switch (button_type_) {
    case VIEW_ID_MINIMIZE_BUTTON: {
      const int y = symbol_rect.CenterPoint().y();
      const gfx::Point p1 = gfx::Point(symbol_rect.x(), y);
      const gfx::Point p2 = gfx::Point(symbol_rect.right(), y);
      canvas->DrawLine(p1, p2, flags);
      return;
    }

    case VIEW_ID_MAXIMIZE_BUTTON:
      DrawRect(canvas, symbol_rect, flags);
      return;

    case VIEW_ID_RESTORE_BUTTON: {
      // Bottom left ("in front") square.
      const int separation = std::floor(2 * scale);
      symbol_rect.Inset(0, separation, separation, 0);
      DrawRect(canvas, symbol_rect, flags);

      // Top right ("behind") square.
      canvas->ClipRect(symbol_rect, SkClipOp::kDifference);
      symbol_rect.Offset(separation, -separation);
      DrawRect(canvas, symbol_rect, flags);
      return;
    }

    case VIEW_ID_CLOSE_BUTTON: {
      flags.setAntiAlias(true);
      // The close button's X is surrounded by a "halo" of transparent pixels.
      // When the X is white, the transparent pixels need to be a bit brighter
      // to be visible.
      const float stroke_halo =
          stroke_width * (symbol_color == SK_ColorWHITE ? 0.1f : 0.05f);
      flags.setStrokeWidth(stroke_width + stroke_halo);

      // TODO(bsep): This sometimes draws misaligned at fractional device scales
      // because the button's origin isn't necessarily aligned to pixels.
      canvas->ClipRect(symbol_rect);
      SkPath path;
      path.moveTo(symbol_rect.x(), symbol_rect.y());
      path.lineTo(symbol_rect.right(), symbol_rect.bottom());
      path.moveTo(symbol_rect.right(), symbol_rect.y());
      path.lineTo(symbol_rect.x(), symbol_rect.bottom());
      canvas->DrawPath(path, flags);
      return;
    }

    default:
      NOTREACHED();
      return;
  }
}
}  // namespace electron