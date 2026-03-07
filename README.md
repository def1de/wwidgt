# wwidgt

**wwidgt** is a GTK4-based widget framework for **wlroots-based** Wayland compositors (primarily **Hyprland**), written in C.

It renders a window using `gtk4-layer-shell` and builds its UI from an **XML layout file** in your user config directory.

## Install (Arch / AUR)

`wwidgt` is published on the AUR as `wwidgt`: https://aur.archlinux.org/packages/wwidgt

Using an AUR helper (example with `yay`):

```bash
yay -S wwidgt
```

Or with `paru`:

```bash
paru -S wwidgt
```

## Runtime dependencies

At runtime `wwidgt` relies on:

- `gtk4`
- `gtk4-layer-shell`
- `libxml2`

(Your AUR build should pull these in automatically.)

## Configuration

On startup, `wwidgt` reads its config from:

- `$XDG_CONFIG_HOME/wwidgt/` (usually `~/.config/wwidgt/`)

The following files are expected:

- `layout.xml` — UI layout definition (XML)
- `style.css` — GTK CSS styling

### Create config directory

```bash
mkdir -p ~/.config/wwidgt
```

### Example `layout.xml`

Create `~/.config/wwidgt/layout.xml`:

```xml
<config>
  <!-- The window tag controls layer-shell behavior -->
  <window layer="top">
    <hbox class="bar">
      <clock format="hh:mm:ss" update="1000" class="clock"/>
      <label text="Hello from wwidgt" class="label"/>
    </hbox>
  </window>
</config>
```

Notes:

- `<window layer="...">` supports: `background`, `bottom`, `top`, `overlay`.
- `wwidgt` builds widgets from tags like `hbox`, `vbox`, `label`, `image`, `button`, `clock`, and progress bar widgets (see `src/ui/` in the repo).

### Example `style.css`

Create `~/.config/wwidgt/style.css`:

```css
.bar {
  padding: 8px 12px;
  background: rgba(20, 20, 20, 0.70);
  border-radius: 10px;
}

.clock {
  font-weight: 700;
  margin-right: 12px;
}

.label {
  opacity: 0.9;
}
```

## Run

Start `wwidgt` from a terminal:

```bash
wwidgt
```

If the widget window doesn’t appear, make sure you’re running a wlroots compositor and have `layout.xml` + `style.css` in `~/.config/wwidgt/`.

## Build from source (optional)

```bash
sudo pacman -S --needed cmake pkgconf gcc gtk4 gtk4-layer-shell libxml2
git clone https://github.com/iktrnch/wwidgt
cd wwidgt
cmake -S . -B build
cmake --build build
sudo cmake --install build
```
