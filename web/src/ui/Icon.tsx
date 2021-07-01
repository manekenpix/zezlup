export const Icon = (props: {icon: string; [k: string]: unknown}) => {
  return (
    <span {...props}>
      <span
        className="iconify"
        data-icon={`ci:${props.icon}`}
        w="full"
        h="full"
      />
    </span>
  );
};
