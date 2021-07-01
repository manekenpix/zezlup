export class ImageLoader {
  readonly image: HTMLImageElement;
  onStateChange?: Function;

  constructor(source: string) {
    this.image = new Image();
    this.image.onload = () => this.onStateChange?.();
    this.load(source);
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
