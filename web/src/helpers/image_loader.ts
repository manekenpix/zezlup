export class ImageLoader {
  readonly image: HTMLImageElement;
  onStateChange?: Function;

  constructor() {
    this.image = new Image();
    this.image.onload = () => this.onStateChange?.();
  }

  get imageSize(): Vec2 {
    return [this.image.width, this.image.height];
  }

  load(value?: string | File) {
    if (!value) return;
    if (typeof value === 'string') {
      this.image.src = value;
      return;
    }

    const reader = new FileReader();
    reader.onloadend = () => (this.image.src = String(reader.result));
    reader.readAsDataURL(value);
  }
}
